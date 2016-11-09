#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>

using namespace llvm;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 

    CAT() : FunctionPass(ID) {}

    bool doInitialization (Module &M) override {
      errs() << "CATPass: doInitialization for \"" << M.getName() <<"\"\n";
      return false;
    }

    bool runOnFunction (Function &F) override {
      bool modified = false;
      errs() << "CATPass: Start\n";
      for (auto& B : F) {
        for (auto& I : B) {
          if (auto* call = dyn_cast<CallInst>(&I)) {
            Function *callee;
            Type *retType;
            errs() << "CATPass:   Found a call instruction\n";

            // Get the function called
            callee = call->getCalledFunction();

            // Get the return type of the callee
            retType = callee->getReturnType();

            // Consider only integer types
            if (auto *intType = dyn_cast<IntegerType>(retType)){
              Value *constValue;
              errs() << "CATPass:   Found a call instruction with integer as return type\n";

              // Create a constant that will replace the return value of the call
              constValue = ConstantInt::get(intType, 42, true);

              // Replace every use of the value generated by the call instruction with our constant
              call->replaceAllUsesWith(constValue);

              // Keep track of changes
              modified = true;
            }
          }
        }
      }

      errs() << "CATPass: End\n";
      return modified;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      errs() << "CATPass: getAnalysisUsage\n";
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