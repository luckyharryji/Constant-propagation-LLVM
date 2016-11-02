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
      map<Instruction *, Value *> add_instruction_to_argument;
      map<Value *, Instruction *> argument_to_add_instruction;
      Constant *zeroConst = ConstantInt::get(IntegerType::get(currentModule->getContext(), 32), 0, true);
      Instruction *insert_point = dyn_cast<Instruction>(F.getEntryBlock().getFirstInsertionPt());

      for (auto iter = F.arg_begin(); iter != F.arg_end(); iter++) {
        // used to mark a create instruction for every argument value for the
        // function
        // So each CAT data pass to the function parameter which is
        // outside of the scope will have a add instruction marked to be used
        // as the create function for the CAT data, then applied to gen set
        Instruction *add_inst = BinaryOperator::Create(Instruction::Add, zeroConst,
                                                      zeroConst, "", insert_point);
        argument_to_add_instruction[dyn_cast<Value>(iter)] = add_inst;
        add_instruction_to_argument[add_inst] = dyn_cast<Value>(iter);
      }
      for (auto &B : F) {
        for (auto &I : B) {

          instruction_index[&I] = instruction_list.size();
          instruction_list.push_back(&I);
          predecessor[&I] = set<Instruction *>();
          in_set_map[&I] = set<Instruction *>();
          out_set_map[&I] = set<Instruction *>();

          if (auto* call_inst = dyn_cast<CallInst>(&I)) {
            Function *function_callled = call_inst->getCalledFunction();
            if (CAT_functions.find(function_callled) != CAT_functions.end()) {
              if (isVariableCreated(function_callled)) {
                variable_used[&I] = set<Instruction *>();
                variable_used[&I].insert(&I);
              } else if (isVariableChanged(function_callled)) {
                Value* first_argument = call_inst->getArgOperand(0);
                variable_used[dyn_cast<Instruction>(first_argument)].insert(&I);
                if (isa<Argument>(first_argument)) {
                  variable_used[dyn_cast<Instruction>(first_argument)].insert(
                    argument_to_add_instruction[first_argument]
                  );
                }
              }
            }
          }
        }
      }

      for (auto &B : F) {
        for (auto &I : B) {
          gen_set_map[&I] = set<Instruction *>();
          kill_set_map[&I] = set<Instruction *>();

          if (
            add_instruction_to_argument.find(&I)
              != add_instruction_to_argument.end()
            || isa<PHINode>(&I)
          ) {
            gen_set_map[&I].insert(&I);
          } else if(auto* store_inst = dyn_cast<StoreInst>(&I)) {
            Value* stored_value = store_inst->getValueOperand();
            if (!isa<Argument>(stored_value)) {
              kill_set_map[&I].insert(dyn_cast<Instruction>(stored_value));
            }
          } else if (auto* call_inst = dyn_cast<CallInst>(&I)) {
            Function *function_callled = call_inst->getCalledFunction();
            if (isVariableCreated(function_callled)) {
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
            } else if (CAT_functions.find(function_callled) == CAT_functions.end()){
              // This is used to handle the function which call the function
              // that use CAT DATA as argument, such as:
              /*
                ...
                d1 = CAT_create_signed_value(1);
                foo(d1);
                printf(CAT_get_signed_value(d1));
              */
              // for conservative, all of these cases do not propagate the constant
              // of the value
              int num_operand = call_inst->getNumArgOperands();
              for (int i = 0; i < num_operand; i++) {
                Value* argument = call_inst->getArgOperand(i);
                if (auto* arg_create_inst = dyn_cast<Instruction>(call_inst->getArgOperand(i))) {
                  if (isInstructionModifyVariable(arg_create_inst)) {
                    kill_set_map[&I].insert(arg_create_inst);
                  }
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

      // store the instruction that can be transformed into constant value
      // save the instruction , value pair
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
                Value* argument = call_inst->getArgOperand(0);
                Instruction *potentialCreateInstruction = NULL;

                for (
                  auto inst = in_set_map[&I].begin();
                  inst != in_set_map[&I].end();
                  ++inst
                ) {
                  if (add_instruction_to_argument.find(*inst) != add_instruction_to_argument.end()) {
                    if (add_instruction_to_argument[*inst] == argument) {
                      potentialCreateInstruction = NULL;
                      break;
                    }
                  } else if (isa<PHINode>(*inst)) {
                    auto phi_node = dyn_cast<PHINode>(*inst);
                    int num_phi_node = phi_node->getNumIncomingValues();
                    Value* temp_upstream_value = NULL;
                    bool valid = true;
                    for (int i = 0; i < num_phi_node; i++) {
                      if (temp_upstream_value == NULL) {
                        temp_upstream_value = phi_node->getIncomingValue(i);
                      } else {
                        bool check = false;
                        auto new_upstream_value = phi_node->getIncomingValue(i);
                        if (auto* left_calll_inst = dyn_cast<CallInst>(temp_upstream_value)) {
                          // errs() << "okay to here";
                          if (auto *right_call_inst = dyn_cast<CallInst>(new_upstream_value)) {
                            Function *left_called_function = left_calll_inst->getCalledFunction(),
                                     *right_called_function = right_call_inst->getCalledFunction();
                            if (isVariableCreated(left_called_function) && isVariableCreated(right_called_function)) {
                              if (left_calll_inst->getArgOperand(0) == right_call_inst->getArgOperand(0)) {
                                check = true;
                              }
                            }
                          }
                        }
                        valid = valid && check;
                      }
                    }
                    if (valid == true) {
                      potentialCreateInstruction = dyn_cast<Instruction>(temp_upstream_value);
                    }
                  } else if (auto *inside_call_inst = dyn_cast<CallInst>(*inst)) {
                    Function *inset_function = inside_call_inst->getCalledFunction();
                    if (
                      isVariableCreated(inset_function)
                    ) {
                      if (def_instruction == *inst) {
                        potentialCreateInstruction = *inst;
                      }
                    } else if (isVariableChanged(inset_function)) {
                      if (call_inst->getArgOperand(0) == inside_call_inst->getArgOperand(0)) {
                        potentialCreateInstruction = NULL;
                        break;
                      }
                    }
                  }
                }
                if (potentialCreateInstruction != NULL) {
                  CallInst* create_inst =
                    dyn_cast<CallInst>(potentialCreateInstruction);
                  Value* const_value = create_inst->getArgOperand(0);
                  if (isa<ConstantInt>(const_value)) {
                    replace_pair[&I] = dyn_cast<ConstantInt>(const_value);
                  }
                }
              }
            }
          }
        }
      }

      for (auto& instruction_constant : replace_pair) {
        Instruction* replace_instruction = instruction_constant.first;
        BasicBlock::iterator ii(replace_instruction);
        ReplaceInstWithValue(
          replace_instruction->getParent()->getInstList(),
          ii,
          instruction_constant.second
        );
      }

      for (auto& add_instruction_pair : add_instruction_to_argument) {
        (add_instruction_pair.first)->eraseFromParent();
      }

      replace_pair.clear();
      gen_set_map.clear();
      kill_set_map.clear();
      variable_used.clear();
      in_set_map.clear();
      out_set_map.clear();
      instruction_list.clear();
      instruction_index.clear();
      predecessor.clear();
      add_instruction_to_argument.clear();
      argument_to_add_instruction.clear();

      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      // errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      AU.setPreservesAll();
    }

  private:
    bool isVariableCreated(Function* function) {
      return
        currentModule->getFunction("CAT_create_signed_value") == function;
    }

    bool isVariableChanged(Function* function) {
      if (
        currentModule->getFunction("CAT_binary_add") == function
        || currentModule->getFunction("CAT_binary_sub") == function
      ) {
        return true;
      }
      return false;
    }

    bool isInstructionModifyVariable(Instruction* I) {
      if (auto* call_inst = dyn_cast<CallInst>(I)) {
        Function *function_callled = call_inst->getCalledFunction();
        if (CAT_functions.find(function_callled) != CAT_functions.end()) {
          if (
            isVariableCreated(function_callled) || isVariableChanged(function_callled)
          ) {
            return true;
          }
        }
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
