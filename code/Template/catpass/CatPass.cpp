#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/SparseBitVector.h"
#include "llvm/IR/Constants.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

using namespace llvm;
using namespace std;

namespace {
  struct CAT : public FunctionPass {
    static char ID;
    Module *currentModule;
    const vector<string> CAT_function_list = {
      "CAT_binary_add", "CAT_binary_sub", "CAT_create_signed_value",
      "CAT_get_signed_value",
    };
    set<Function *> CAT_functions;

    CAT() : FunctionPass(ID) {}

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      currentModule = &M;
      for (auto& function_name : CAT_function_list) {
        CAT_functions.insert(M.getFunction(function_name));
      }
      return false;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      map<Instruction *, set<Instruction *>> gen_set_map;
      map<Instruction *, set<Instruction *>> kill_set_map;
      map<Instruction *, set<Instruction *>> variable_used;
      map<Instruction *, set<Instruction *>> in_set_map;
      map<Instruction *, set<Instruction *>> out_set_map;
      vector<Instruction *> instruction_list;
      map<Instruction *, int> instruction_index;
      map<Instruction *, set<Instruction *>> predecessor;
      for (auto &B : F) {
        for (auto &I : B) {

          instruction_index[&I] = instruction_list.size();
          instruction_list.push_back(&I);
          predecessor[&I] = set<Instruction *>();
          in_set_map[&I] = set<Instruction *>();
          out_set_map[&I] = set<Instruction *>();

          if (auto* call_inst = dyn_cast<CallInst>(&I)) {
            Function *function_callled;
            function_callled = call_inst->getCalledFunction();
            if (CAT_functions.find(function_callled) != CAT_functions.end()) {
              if (
                currentModule
                  ->getFunction("CAT_create_signed_value") == function_callled
              ) {
                variable_used[&I] = set<Instruction *>();
                variable_used[&I].insert(&I);
              }
              else if (isVariableChanged(function_callled)) {
                variable_used[dyn_cast<Instruction>(call_inst->getArgOperand(0))].insert(&I);
              }
            }
          }
        }
      }

      for (auto &B : F) {
        for (auto &I : B) {
          gen_set_map[&I] = set<Instruction *>();
          kill_set_map[&I] = set<Instruction *>();

          if (auto* call_inst = dyn_cast<CallInst>(&I)) {
            Function *function_callled;
            function_callled = call_inst->getCalledFunction();
            if (
              currentModule
                ->getFunction("CAT_create_signed_value") == function_callled
            ) {
              gen_set_map[&I].insert(&I);
              for (
                auto inst = variable_used[&I].begin();
                inst != variable_used[&I].end();
                ++inst
              ) {
                if (*inst != &I) {
                  kill_set_map[&I].insert(*inst);
                }
              }
            } else if (isVariableChanged(function_callled)) {
              gen_set_map[&I].insert(&I);
              Instruction * killed_instruction =
                dyn_cast<Instruction>(call_inst->getArgOperand(0));
              for (
                auto inst = variable_used[killed_instruction].begin();
                inst != variable_used[killed_instruction].end();
                ++inst
              ) {
                if (*inst != &I) {
                  kill_set_map[&I].insert(*inst);
                }
              }
            }
          }
        }
      }

      for (auto &B : F) {
        // predecessor of the first instruction of a basic block is the
        // last instruction of the block predecessor
        for (auto it = pred_begin(&B), et = pred_end(&B); it != et; ++it)
        {
          predecessor[&(B.front())].insert((*it)->getTerminator());
        }
        // not doing first instruction of block, since it is dealt with
        for (auto iter = ++B.begin(); iter != B.end(); ++iter) {
          // predecessor of the instruction inside the basic block which is not
          // the first instruction is the previous instruction
          predecessor[&(*iter)].insert(
            instruction_list[instruction_index[&(*iter)] - 1]
          );
        }
      }

      bool changed = true;
      while (changed) {
        changed = false;
        for (auto &B : F) {
          for (auto &I : B) {
            set<Instruction *> new_in;
            if (!predecessor[&I].empty()) {
              for (
                auto inst = predecessor[&I].begin();
                inst != predecessor[&I].end();
                ++inst
              ) {
                new_in.insert(out_set_map[*inst].begin(), out_set_map[*inst].end());
              }
            }
            set<Instruction *> new_out;
            set<Instruction *> temp_in;
            temp_in.insert(new_in.begin(), new_in.end());
            // IN - KILL
            if (!kill_set_map[&I].empty()) {
              for (
                auto inst = kill_set_map[&I].begin();
                inst != kill_set_map[&I].end();
                ++inst
              ) {
                if (temp_in.find(*inst) != temp_in.end()) {
                  temp_in.erase(*inst);
                }
              }
            }
            new_out.insert(temp_in.begin(), temp_in.end());
            if (!gen_set_map[&I].empty()) {
              new_out.insert(gen_set_map[&I].begin(), gen_set_map[&I].end());
            }
            in_set_map[&I] = new_in;
            if (out_set_map[&I] != new_out) {
              changed = true;
              out_set_map[&I] = new_out;
            }
            new_in.clear();
            new_out.clear();
            temp_in.clear();
          }
        }
      }

      map<Instruction*, ConstantInt*> replace_pair;
      for (auto &B : F) {
        for (auto &I : B) {
          if (!in_set_map[&I].empty()) {
            if (auto* call_inst = dyn_cast<CallInst>(&I)) {
              Function *function_callled;
              function_callled = call_inst->getCalledFunction();
              if (
                currentModule
                  ->getFunction("CAT_get_signed_value") == function_callled
              ) {
                Instruction* def_instruction =
                  dyn_cast<Instruction>(call_inst->getArgOperand(0));
                Instruction *reachingDef = NULL;

                for (
                  auto inst = in_set_map[&I].begin();
                  inst != in_set_map[&I].end();
                  ++inst
                ) {
                  if (auto *inside_call_inst = dyn_cast<CallInst>(*inst)) {
                    Function *inset_function = inside_call_inst->getCalledFunction();
                    if (
                      currentModule->getFunction("CAT_create_signed_value") == inset_function
                    ) {
                      if (def_instruction == *inst) {
                        reachingDef = *inst;
                      }
                    } else if (isVariableChanged(inset_function)) {
                      if (call_inst->getArgOperand(0) == inside_call_inst->getArgOperand(0)) {
                        reachingDef = NULL;
                        break;
                      }
                    }
                  }
                }
                if (reachingDef != NULL) {
                  CallInst *v_to_c = dyn_cast<CallInst>(reachingDef);
                  Value *v = v_to_c->getArgOperand(0);
                  if (isa<ConstantInt>(v)) {
                    ConstantInt *const_int = dyn_cast<ConstantInt>(v);
                    replace_pair[&I] = dyn_cast<ConstantInt>(v);
                    //c = cnst->getSExtValue();
                    // errs() << I << '\n';
                    // // errs() << "Function is : " << F << "\n";
                    // errs() << *const_int << '\n';
                    // errs() << "whole function" << '\n';
                    // errs() << F << '\n';
                    // replace all use of var with constant
                    Instruction* replace_instruction = &I;
                    BasicBlock::iterator ii(&I);
                    BasicBlock* test_block = &B;
                    //Value *const_val(cnst->getType(), c);
                    // ReplaceInstWithValue((*bb->getInstList(), ii, cnst);
                    // ReplaceInstWithValue(test_block->getInstList(), ii, const_int);

                    // Instruction &I_replace = *ii;
                    // // errs() << "Replace Instruction" << I_replace << '\n';
                    // // Replaces all of the uses of the instruction with uses of the value
                    // I.replaceAllUsesWith(const_int);
                    // std::string OldName = I.getName();
                    // // errs() << OldName << '\n';
                    // // // Delete the unnecessary instruction now...
                    // // ii = test_block->getInstList().erase(ii);
                    // // I.eraseFromParent();
                    // // errs() << test_block->getInstList() << '\n';
                    // for (auto instruction_test = test_block->getInstList().begin(); instruction_test != test_block->getInstList().end(); instruction_test++) {
                    //   Instruction* print_instruction= dyn_cast<Instruction>(instruction_test);
                    //   errs() << *print_instruction << '\n';
                    // }
                    // // ii = test_block->getInstList().erase(ii)
                    // // // Make sure to propagate a name if there is one already.
                    // if (!OldName.empty() && !const_int->hasName())
                    //   const_int->setName(OldName);
                  }
                }
              } else if (
                isVariableChanged(function_callled)
              ) {

              }
            }
          }
        }
      }

      for (auto& instruction_constant  : replace_pair) {
        Instruction* replace_instruction = instruction_constant.first;
        BasicBlock::iterator ii(replace_instruction);
        // BasicBlock* test_block = &B;
        //Value *const_val(cnst->getType(), c);
        // ReplaceInstWithValue((*bb->getInstList(), ii, cnst);
        ReplaceInstWithValue(replace_instruction->getParent()->getInstList(), ii, instruction_constant.second);
        // ReplaceInstWithValue(test_block->getInstList(), ii, const_int);
      }
      // errs() << "START FUNCTION: " << F.getName() << '\n';
      // for (auto &B : F) {
      //   for (auto &I : B) {
      //     errs() << "INSTRUCTION: " << I << '\n';
      //     errs() << "***************** IN\n" << "{\n";
      //     if (!in_set_map[&I].empty()) {
      //       for (
      //         auto inst = in_set_map[&I].begin();
      //         inst != in_set_map[&I].end();
      //         ++inst
      //       ) {
      //         errs() << ' ' << **inst << '\n';
      //       }
      //     }
      //     errs() << "}\n" << "**************************************\n";
      //     errs() << "***************** OUT\n" << "{\n";
      //     if (!out_set_map[&I].empty()) {
      //       for (
      //         auto inst = out_set_map[&I].begin();
      //         inst != out_set_map[&I].end();
      //         ++inst
      //       ) {
      //         errs() << ' ' << **inst << '\n';
      //       }
      //     }
      //     errs() << "}\n" << "**************************************\n"
      //            << "\n\n\n";
      //   }
      // }

      gen_set_map.clear();
      kill_set_map.clear();
      variable_used.clear();
      in_set_map.clear();
      out_set_map.clear();
      instruction_list.clear();
      instruction_index.clear();
      predecessor.clear();

      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      // errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      AU.setPreservesAll();
    }

  private:
    bool isVariableChanged(Function* function) {
      if (
        currentModule->getFunction("CAT_binary_add") == function
        || currentModule->getFunction("CAT_binary_sub") == function
      ) {
        return true;
      }
      return false;
    }
  };
}

// Next there is code to register your pass to "opt"
char CAT::ID = 0;
static RegisterPass<CAT> X("CAT", "Homework for the CAT class");

// Next there is code to register your pass to "clang"
static CAT * _PassMaker = NULL;
static RegisterStandardPasses _RegPass1(PassManagerBuilder::EP_OptimizerLast,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT());}}); // ** for -Ox
static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT()); }}); // ** for -O0
