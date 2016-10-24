#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <set>
#include <list>
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
      return false;
    }

    bool runOnFunction (Function &F) override {
      errs() << "CATPass: Start\n" ;
      errs() << "CATPass:   Current function \"" << F.getName() << "\"\n" ;
      for (auto *U : F.users()) {
        if (auto *Inst = dyn_cast<Instruction>(U)) {
          auto *caller = Inst->getFunction();
          errs() << "CATPass:     Used by instruction " << *Inst << " of function \"" << caller->getName() << "\"\n";
        }
      }
      errs() << "CATPass: End\n";
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
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
