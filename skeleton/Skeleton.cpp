#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      errs() << "I saw a function called " << F.getName() << "!\n";
      for (auto &b : F)
      {
        for (auto &I : b)
        {
          //This cast will only succeed, if we get a binary opeartor
          //A successful cast will also produce a valid poiniter, triggering our IF block
          if(auto *OP = dyn_cast<BinaryOperator>(&I)){
            errs() << "=======A Binary Instruction found =======\n";
            errs() << *OP << "\n";

            //Lets modify this binary opeartor instruction
            //Whatever, this instruction is currently doing, we will make it do the multiplication instead.
            IRBuilder<> builder(OP);

            //We only have LHS and RHS, since this is a BinaryOperator
            Value *lhs = OP->getOperand(0);
            Value *rhs = OP->getOperand(1);

            Value *mul = builder.CreateMul(lhs, rhs);

            //At this point we have created a brand new Multiplication instruction using LLVM
            //Now we need to replace the old instruction currently pointed by `OP` with the new one that is `mul`
            //It can be done by getting the list of all the users who are currently consuming the result of OP (where its being used) 
            //And then for those users updating the old instruction with the new one i-e `MUL`
            for (auto &U : OP->uses()){
              User *user = U.getUser();

              user->setOperand(U.getOperandNo(), mul);
            }
            
          }
        }
      }

      return false;
    }

  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
