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
      for (auto &B : F) {
        for (auto &I : B) {
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
              else if (
                currentModule->getFunction("CAT_binary_add") == function_callled
                || currentModule->getFunction("CAT_binary_sub") == function_callled
              ) {
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
            }
            else if (
              currentModule->getFunction("CAT_binary_add") == function_callled
              || currentModule->getFunction("CAT_binary_sub") == function_callled
            ) {
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

      errs() << "START FUNCTION: " << F.getName() << '\n';
      for (auto &B : F) {
        for (auto &I : B) {
          errs() << "INSTRUCTION: " << I << '\n';
          errs() << "***************** GEN\n" << "{\n";
          if (!gen_set_map[&I].empty()) {
            for (
              auto inst = gen_set_map[&I].begin();
              inst != gen_set_map[&I].end();
              ++inst
            ) {
              errs() << ' ' << **inst << '\n';
            }
          }
          errs() << "}\n" << "**************************************\n";
          errs() << "***************** KILL\n" << "{\n";
          if (!kill_set_map[&I].empty()) {
            for (
              auto inst = kill_set_map[&I].begin();
              inst != kill_set_map[&I].end();
              ++inst
            ) {
              errs() << ' ' << **inst << '\n';
            }
          }
          errs() << "}\n" << "**************************************\n"
                 << "\n\n\n";
        }
      }
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      // errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      AU.setPreservesAll();
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
