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
#include "llvm/Transforms/Utils/Cloning.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <tuple>
#include "llvm/Analysis/DependenceAnalysis.h"

using namespace llvm;
using namespace std;

namespace {

  struct condition_result {
    Value* first_condition;
    Value* second_condition;
  };

  struct cmp_inst_status {
    llvm::CmpInst::Predicate cmp_operand;
    int64_t value;
    condition_result result_info;
  };

  struct cloned_function_copy {
    Function* true_condition;
    Function* false_condition;
  };

  struct CAT : public ModulePass {
    static char ID;
    Module *currentModule;
    const vector<string> CAT_function_list = {
      "CAT_binary_add", "CAT_binary_sub", "CAT_create_signed_value",
      "CAT_get_signed_value",
    };
    set<Function *> CAT_functions;
    map<Function *, Value *> constant_return;
    map<Function *, cmp_inst_status> function_arg_info;
    map<Function *, cloned_function_copy> function_copy;

    CAT() : ModulePass(ID) { }

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      currentModule = &M;
      for (auto& function_name : CAT_function_list) {
        // Initialize the list of function shown in the input module
        CAT_functions.insert(M.getFunction(function_name));
      }
      return false;
    }

    bool runOnModule(Module &M) override {
      map<Function *, set<Function *>> call_graph;
      for (auto &F : M) {
        call_graph[&F] = set<Function *>();
      }
      for (auto &F : M) {
        for (auto &B : F){
          for (auto &I : B){
            if (auto call = dyn_cast<CallInst>(&I)){
              call_graph[&F].insert(call->getCalledFunction());
            }
          }
        }
      }

      for (auto &F : M) {
        functionSummary(F);
      }

      for (auto &F : M) {
        if (function_arg_info.find(&F) != function_arg_info.end()) {
          cloneFunctionCopy(F);
        }
      }

      bool value_propagate;
      do {
        value_propagate = false;
        for (auto &F : M) {
          if (runOnIntraFunction(F)) {
            value_propagate = true;
          }
        }
      } while (value_propagate);

      return false;
    }

    void cloneFunctionCopy(Function &F) {
      if (!F.empty()) {
        BasicBlock &entry_block = F.front();
        if (auto* branch_inst = (&entry_block)->getTerminator()) {
          errs() << "first block: " << entry_block << "\n";
          Function* true_cloned = clonedNewFunction(&F, true);
          Function* false_cloned = clonedNewFunction(&F, false);
          cloned_function_copy function_clone_result;
          function_clone_result.true_condition = true_cloned;
          function_clone_result.false_condition = false_cloned;
          function_copy[&F] = function_clone_result;
        }
      }
    }

    Function* clonedNewFunction(Function* callee, bool condition) {
      errs() << "Cloning " << callee->getName() << "\n";
      ValueToValueMapTy VMap;
      auto *clonedCallee = CloneFunction(callee, VMap, true, nullptr);
      clonedCallee->setLinkage(callee->getLinkage());
      removeUnuseCondition(clonedCallee, condition);
      if (condition == true) {
        clonedCallee->setName(callee->getName() + "_true_cloned");
      } else {
        clonedCallee->setName(callee->getName() + "_false_cloned");
      }
      errs() << "after clone, result function is : " << condition << *clonedCallee << '\n';
      return clonedCallee;
      // call->replaceUsesOfWith(callee, clonedCallee);
      // M.getFunctionList().push_back(clonedCallee);
    }

    void removeUnuseCondition(Function* F, bool condition) {
      if (!F->empty()) {
        BasicBlock &entry_block = F->front();
        if (auto* branch_inst = (&entry_block)->getTerminator()) {
          BranchInst* new_branch;
          BasicBlock* delete_block;
          if (condition == true) {
            new_branch = BranchInst::Create(branch_inst->getSuccessor(0));
            delete_block = branch_inst->getSuccessor(1);
          } else {
            new_branch = BranchInst::Create(branch_inst->getSuccessor(1));
            delete_block = branch_inst->getSuccessor(0);
          }
          ReplaceInstWithInst(branch_inst, new_branch);
          DeleteDeadBlock(delete_block);
        }
      }
    }

    bool functionSummary(Function &F) {
      for (auto &B : F){
        for (auto &I : B){
          if (isa<ReturnInst>(&I)) {
            errs() << "return Instruction: " << I << "\n";
            ReturnInst *return_inst = dyn_cast<ReturnInst>(&I);
            Value *returned_value = return_inst->getReturnValue();
            // errs() << "return Value: " << returned_value << "\n";
            if (returned_value != NULL && isa<Instruction>(returned_value)) {
              if (auto *instruc_called_return = dyn_cast<Instruction>(returned_value)){
                errs() << "return Instruction: " << *instruc_called_return << "\n";
                if (instruc_called_return != NULL && isa<CallInst>(instruc_called_return)) {
                  if (auto *call_inst = dyn_cast<CallInst>(instruc_called_return)) {
                    Function *function_callled = call_inst->getCalledFunction();
                    if (CAT_functions.find(function_callled) != CAT_functions.end()) {
                      if (isVariableCreated(function_callled)) {
                        errs() << "called create Instruction: " << *call_inst << "\n";
                        constant_return[&F] = call_inst->getArgOperand(0);
                      }
                    }
                  }
                } else if (isa<PHINode>(instruc_called_return)) {
                  auto phi_node = dyn_cast<PHINode>(instruc_called_return);
                  int num_phi_node = phi_node->getNumIncomingValues();
                  BranchInst* temp_branch_source = NULL;
                  CmpInst* temp_compare_source = NULL;
                  bool same_branch = true;
                  for (int i = 0; i < num_phi_node; i++) {
                    if (auto* phi_calll_inst = dyn_cast<CallInst>(phi_node->getIncomingValue(i))) {
                      if (isVariableCreated(phi_calll_inst->getCalledFunction())) {
                        if (auto *branch_block = (phi_calll_inst->getParent())->getSinglePredecessor()) {
                          if (auto *branch_inst = dyn_cast<BranchInst>((branch_block->getTerminator()))) {
                            if (temp_branch_source == NULL) {
                              temp_branch_source = branch_inst;
                            } else {
                              if (temp_branch_source != branch_inst) {
                                same_branch = false;
                              }
                            }
                            Value *condition = branch_inst->getCondition();
                            if (auto *compare_inst = dyn_cast<CmpInst>(condition)) {
                              if (temp_compare_source == NULL) {
                                temp_compare_source = compare_inst;
                              } else {
                                if (temp_compare_source != compare_inst) {
                                  same_branch = false;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  if (same_branch == false || temp_compare_source == NULL
                      || temp_branch_source == NULL) {
                    continue;
                  }
                  cmp_inst_status cmp_status;
                  handleCompare(&F, temp_compare_source, cmp_status);
                  condition_result inside_condition_result;
                  errs() << "compare instruction: " << cmp_status.cmp_operand << " " << cmp_status.value << "\n";
                  for (int i = 0; i < num_phi_node; i++) {
                    if (auto* phi_calll_inst = dyn_cast<CallInst>(phi_node->getIncomingValue(i))) {
                      errs() << "phi node that call other function" << *phi_calll_inst << "\n";
                      if (isVariableCreated(phi_calll_inst->getCalledFunction())) {
                        errs() << "phi node that call create CAT function" << *phi_calll_inst << "\n";
                        if (auto *branch_block = (phi_calll_inst->getParent())->getSinglePredecessor()) {
                          errs() << "branching_instruction is: " << *(branch_block->getTerminator()) << '\n';
                          if (auto *branch_inst = dyn_cast<BranchInst>((branch_block->getTerminator()))) {
                            Value *condition = branch_inst->getCondition();
                            errs() << "Condition is : " << *condition << "\n";
                            if (phi_calll_inst->getParent() == branch_inst->getSuccessor(0)) {
                              errs() << "true condition" << "\n";
                              inside_condition_result.first_condition = phi_calll_inst->getArgOperand(0);
                            } else {
                              errs() << "false condition" << "\n";
                              inside_condition_result.second_condition = phi_calll_inst->getArgOperand(0);
                            }
                          }
                        }
                      }
                    }
                  }
                  cmp_status.result_info = inside_condition_result;
                  function_arg_info[&F] = cmp_status;
                }
              }
            }
          }
        }
      }
      return false;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnIntraFunction (Function &F) {
      if (F.isDeclaration()) {
          return false;
      }
      bool modified = false;

      DependenceAnalysis &deps = getAnalysis<DependenceAnalysis>(F);
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
      set<Instruction*> candidate_list;

      Constant *zeroConst = ConstantInt::get(IntegerType::get(currentModule->getContext(), 32), 0, true);
      Instruction *insert_point = dyn_cast<Instruction>(F.getEntryBlock().getFirstInsertionPt());

      for (auto iter = F.arg_begin(); iter != F.arg_end(); iter++) {
        // used as a 'create' instruction for every argument value for the
        // function,
        // So each CAT variable in the function parameter which is
        // outside of the scope will have an fake add instruction treated as
        // the create function for the CAT variable, then applied the same
        // method as previously to gen / kill set for analysis
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

          // record the defination instruction of the CAT variable
          // including the cat_add/sub/create and the fake add instruction
          // to handle the variable pass to function as argument
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
              // kill_set_map[&I].insert(dyn_cast<Instruction>(stored_value));
              // This case is used to handle the store instruction / pointer read
              // Instead of the conservative way to claim can not propagate and add to kill set,
              // every time loop to a store instruction, add all instruction to
              // a list(set) and then use LLVM depends API to decide whether
              // to propagate the constant in the later in set iteration
              candidate_list.insert(dyn_cast<Instruction>(stored_value));
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
              // This case is used to handle the function that use CAT variable
              // as argument but not a CAT function, such as:
              /*
                ...
                d1 = CAT_create_signed_value(1);
                foo(d1);
                printf(CAT_get_signed_value(d1));
              */
              // for conservative, all of these cases do not propagate the constant
              // of the value,
              // which means will add the CAT variables to the kill set here.
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
              if (def_instruction == NULL) {
                continue;
              }
              if (auto *constant_call = dyn_cast<CallInst>(def_instruction)) {
                Function *get_argument_called = constant_call->getCalledFunction();
                if (constant_return.find(get_argument_called) != constant_return.end()) {
                  if (isa<ConstantInt>(constant_return[get_argument_called])) {
                    replace_pair[&I] = dyn_cast<ConstantInt>(constant_return[get_argument_called]);
                    continue;
                  }
                } else if (function_arg_info.find(get_argument_called) != function_arg_info.end()) {
                  replaceConditionFunction(replace_pair, get_argument_called, &I, argument);
                  continue;
                }
              }

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
                      if (new_upstream_value != NULL) {
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
                    Value *inset_function_variable = inside_call_inst->getArgOperand(0);
                    if (argument == inset_function_variable) {
                      potentialCreateInstruction = NULL;
                      break;
                    } else if (auto *in_set_variable_create = dyn_cast<Instruction>(inset_function_variable)) {
                      if (candidate_list.find(def_instruction) != candidate_list.end()) {
                        // errs() << "can compile to here" << "\n";
                        if (deps.depends(def_instruction, in_set_variable_create, false) != NULL) {
                          potentialCreateInstruction = NULL;
                          break;
                        }
                      }
                    }
                  }
                }
              }
              if (potentialCreateInstruction != NULL) {
                CallInst* create_inst =
                  dyn_cast<CallInst>(potentialCreateInstruction);
                Value* const_value = create_inst->getArgOperand(0);
                if (isa<ConstantInt>(const_value)) {
                  // add all the constant value inside CAT_get function call
                  // replace the Instruction later at once
                  replace_pair[&I] = dyn_cast<ConstantInt>(const_value);
                }
              }
            }
          }
        }
      }

      for (auto& instruction_constant : replace_pair) {
        // replace instruction with constant value at once, constant propagation
        Instruction* replace_instruction = instruction_constant.first;
        BasicBlock::iterator ii(replace_instruction);
        ReplaceInstWithValue(
          replace_instruction->getParent()->getInstList(),
          ii,
          instruction_constant.second
        );
        modified = true;
      }

      // delete the fake add instruction added to the function at the beginning
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

      return modified;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      // errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
      // AU.setPreservesAll();
      AU.addRequiredTransitive<DependenceAnalysis>();
    }

  private:

    void handleCompare(Function* F, CmpInst* compare_inst, cmp_inst_status& required_info) {
      errs() << "compare instruction is: " << *compare_inst << "\n";
      if (compare_inst->getPredicate() == llvm::CmpInst::Predicate::ICMP_SGT) {
        errs() << "great opreand: " << *compare_inst << "\n";
        errs() << "great value0: " << *(compare_inst->getOperand(0)) << "\n";
        errs() << "great value1: " << *(compare_inst->getOperand(1)) << "\n";
        if (needRecord(compare_inst->getOperand(0))) {
          if (auto* constant_value = dyn_cast<ConstantInt>(compare_inst->getOperand(1))) {
            errs() << "is constant int: " << *constant_value << "\n";
            errs() << "value is : " << constant_value->getSExtValue() << "\n";
            required_info.cmp_operand = llvm::CmpInst::Predicate::ICMP_SGT;
            required_info.value = constant_value->getSExtValue();
            // function_arg_info[F] = (constant_value->getSExtValue(), llvm::CmpInst::Predicate::ICMP_SGT);
          }
        }
      } else if (compare_inst->getPredicate() == llvm::CmpInst::Predicate::ICMP_SLT) {
        errs() << "less opreand: " << *compare_inst << "\n";
        errs() << "great value0: " << *(compare_inst->getOperand(0)) << "\n";
        errs() << "less value1: " << *(compare_inst->getOperand(1)) << "\n";
        if (needRecord(compare_inst->getOperand(0))) {
          if (auto* constant_value = dyn_cast<ConstantInt>(compare_inst->getOperand(1))) {
            errs() << "is constant int: " << *constant_value << "\n";
            errs() << "value is : " << constant_value->getSExtValue() << "\n";
            required_info.cmp_operand = llvm::CmpInst::Predicate::ICMP_SLT;
            required_info.value = constant_value->getSExtValue();
            // function_arg_info[F] = (constant_value->getSExtValue(), llvm::CmpInst::Predicate::ICMP_SLT);
          }
        }
      }
    }

    bool needRecord(Value* value_operand) {
      if (auto* first_cmp_call = dyn_cast<CallInst>((value_operand))) {
        if (isVariableGet(first_cmp_call)) {
          Value* first_argument = first_cmp_call->getArgOperand(0);
          if (isa<Argument>(first_argument)) {
            errs() << "Argument is a get instruction with function arg: " << *first_argument << "\n";
            return true;
          }
        }
      } else if (isa<Argument>(value_operand)) {
        errs() << "Argument is the function argument: " << *value_operand << "\n";
        return true;
      }
      return false;
    }

    void replaceConditionFunction(map<Instruction*, ConstantInt*> &replace_pair,
                                  Function* called_function, Instruction* I, Value* get_argument) {
      if (auto* call_inst = dyn_cast<CallInst>(get_argument)) {
        Value* create_argument_inst = call_inst->getArgOperand(0);
        if (auto* create_call_inst = dyn_cast<CallInst>(call_inst->getArgOperand(0))) {
          Function *function_callled;
          function_callled = create_call_inst->getCalledFunction();
          if (
            currentModule
              ->getFunction("CAT_create_signed_value") == function_callled
          ) {
            Value* create_argument = create_call_inst->getArgOperand(0);
            if (auto* constant_create_arg = dyn_cast<ConstantInt>(create_argument)) {
              errs() << "operand is ConstantInt: " << *constant_create_arg <<'\n';
              int index = argumentRange(function_arg_info[called_function], constant_create_arg);
              condition_result replace_result = (function_arg_info[called_function]).result_info;
              Value* replace_value = NULL;
              if (index == 0) {
                errs() << "replace with : " << replace_result.first_condition << '\n';
                replace_value = replace_result.first_condition;
              } else {
                errs() << "replace with : " << replace_result.second_condition << '\n';
                replace_value = replace_result.second_condition;
              }
              if (isa<ConstantInt>(replace_value)) {
                replace_pair[I] = dyn_cast<ConstantInt>(replace_value);
              }
            }
          }
        }
      }
    }

    int replaceConditionFunction_v2(map<Instruction*, ConstantInt*> &replace_pair,
                                  Function* called_function, Instruction* I, Value* get_argument) {
      if (auto* call_inst = dyn_cast<CallInst>(get_argument)) {
        Value* create_argument_inst = call_inst->getArgOperand(0);
        if (auto* create_call_inst = dyn_cast<CallInst>(call_inst->getArgOperand(0))) {
          Function *function_callled;
          function_callled = create_call_inst->getCalledFunction();
          if (
            currentModule
              ->getFunction("CAT_create_signed_value") == function_callled
          ) {
            Value* create_argument = create_call_inst->getArgOperand(0);
            if (auto* constant_create_arg = dyn_cast<ConstantInt>(create_argument)) {
              return argumentRange(function_arg_info[called_function], constant_create_arg);
            }
          }
        }
      }
      return -1;
    }

    int argumentRange(cmp_inst_status function_info, ConstantInt* caller_argument) {
      if (function_info.cmp_operand == llvm::CmpInst::Predicate::ICMP_SLT) {
        if (caller_argument->getSExtValue() < function_info.value) {
          return 0;
        } else {
          return 1;
        }
      } else if (function_info.cmp_operand == llvm::CmpInst::Predicate::ICMP_SGT) {
        if (caller_argument->getSExtValue() > function_info.value) {
          return 0;
        } else {
          return 1;
        }
      }
      return 0;
    }

    bool isVariableGet(CallInst* call_inst) {
      Function *function_callled = call_inst->getCalledFunction();
      if (CAT_functions.find(function_callled) != CAT_functions.end()) {
        if (isVariableGet(function_callled)) {
          return true;
        }
      }
      return false;
    }

    bool isVariableGet(Function* function) {
      return
        currentModule->getFunction("CAT_get_signed_value") == function;
    }

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

    /*
      Input: Instruction
      Output: if instruction modify the variable relates to CAT function,
              either create / add / sub
    */
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
