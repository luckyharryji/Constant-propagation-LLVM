#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Module.h"
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
      errs() << "CATPass: Start\n" ;
      for (inst_iterator iter = inst_begin(F), E = inst_end(F); iter != E; ++iter){
        Instruction &I = *iter;
        I.print(errs());
        errs() << "\n";
      }

      errs() << "CATPass: End\n";
      return false;
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
