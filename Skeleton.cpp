#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/IR/Value.h"

using namespace llvm;

namespace {
  class SkeletonPass : public FunctionPass {

//  class SkeletonPass : public BlockFrequencyInfoWrapperPass {

  public:
    static char ID;
 //   SkeletonPass() : BlockFrequncyInfoWrapperPass() {}
    SkeletonPass() : FunctionPass(ID) {}
    virtual bool runOnFunction(Function &F) {
      std::map<std::string,int> myMap;
      std::map<std::string,int> myMapTemp;

      errs() << "Inside function: " << F.getName() << "!\n";
	for (auto arg = F.arg_begin(), arge = F.arg_end(); arg != arge; ++arg) //For each function argument
	{
   	    Value* address = &*arg;
   	    errs() <<"Function argument's address : " << address << "\n";
          
	  for(Function::iterator itr = F.begin(), itr2 = F.end(); itr != itr2; ++itr) //For each Function
	   {
	    for(BasicBlock::iterator i = itr->begin(), i2 = itr->end();i != i2; ++i) //For each Basic block inside a function
	    {
		////NEWLY ADDED/////
	//	Instruction* inst = dyn_cast<Instruction>(address);
		if(i->getOpcode() == Instruction::Store){
		  Value* add = i->getOperand(1);
	
		for(User *U: add->users()){
		  if(Instruction *Inst = dyn_cast<Instruction>(U)){
	//	     if (inst->getOpcode()==Instruction::Load)
			errs()<<"Instruction used in "<<*Inst<<"\n";
	          }}}
	         // for(User::op_iterator oper = Inst->op_begin(), oper2 = Inst->op_end(); oper != oper2; ++oper)
		//	{errs()<<"Operands : "<<*oper<<"\n"; }
	    	// }//end of IF
		//} //end of for Users
		////////////////////
	      //	errs() << "Arguments "<<i->getArgOperand() <<"\n";
	      if(myMap.find(i->getOpcodeName())== myMap.end()) //Checking if this opcode's first appearance
	       {
                // errs() <<"NAME ="<< i->getOpcodeName() <<"\n";
	           if(strcmp(i->getOpcodeName(),"load")== 0) //Cecking if the opcode is "LOAD"
		    {
		      //CHECKING if(i->getOperand(0)->getType()->isPointerTy())  //Checking if the Operand is of Pointer type
		       { 
		//         errs() << "Pointer Type Load : " <<i->getOperand(0)<<"\n";
		         myMap[i->getOpcodeName()] = 1; // Add count 1 to the Opcode type

		//	errs()<<"Argument add : "<<address<<"\n"<<"load add : "<<i->getOperand(0)<<"\n";
		//	if(address == i->getOperand(0))  // Check if the istruction is accessing memory pointed by our function argument
		//	{ errs()<<"Inside load address"<<"\n";myMapTemp[i->getOpcodeName()] = 1;} 
		      ////////////////////////////////////

	               for(User::op_iterator oper = i->op_begin(), oper2 = i->op_end(); oper != oper2; ++oper)
		          {//errs()<<"Load Operands : "<<*oper<<"\n";
				if(*oper == address)
				 myMapTemp[i->getOpcodeName()] = 1;
			 }
		      ///////////////////////////////////
			 }
		    }

	            else if( strcmp(i->getOpcodeName(),"store")==0) //Checking if the Opcode is "STORE" 
		    {
		     //CHECKING if(i->getOperand(0)->getType()->isPointerTy()) // Checking if the Operand is of Pointer type
                      {
		        // errs() << "Store type Operand : "<< i->getOperand(0) <<" , Operand 2:"<< i->getOperand(1)<<"\n";
		         myMap[i->getOpcodeName()] = 1; //Add 1 to the count of Opcode type

		//	if(address == i->getOperand(0))
		//	{ myMapTemp[i->getOpcodeName()] = 1;
		//	errs()<<"Argument add : "<<address<<"\n"<<"store add : "<<i->getOperand(0)<<"\n";}
		        }
 			  
		      ////////////////////////////////////

	               for(User::op_iterator oper = i->op_begin(), oper2 = i->op_end(); oper != oper2; ++oper)
		          {//errs()<<"Store Operands : "<<*oper<<"\n";

				if(*oper == address)
				 myMapTemp[i->getOpcodeName()] = 1;

			 }
		      ///////////////////////////////////
		      }
		   // }
	       } 
	       else
	       {
		if(i->getOperand(0)->getType())//CHECKING ->isPointerTy()) //Keep adding 1 to the corresponding Opcode count for each relevant appearance
		 { 
			// errs() << "Instruction :"<<i->getOpcodeName()<<" address: "<< i->getOperand(0)<< " memory adress : "<<address<<"\n";; 			

		      ////////////////////////////////////

	           if(strcmp(i->getOpcodeName(),"load")== 0) //Cecking if the opcode is "LOAD"
	           {    for(User::op_iterator oper = i->op_begin(), oper2 = i->op_end(); oper != oper2; ++oper)
		          {//errs()<<"Load Operands : "<<*oper<<"\n";
				
				if(*oper == address)
				 myMapTemp[i->getOpcodeName()]++;
			  }	
			}	
		      ///////////////////////////////////

		      ////////////////////////////////////
		else{
	               for(User::op_iterator oper = i->op_begin(), oper2 = i->op_end(); oper != oper2; ++oper)
		          {//errs()<<"Store Operands : "<<*oper<<"\n";
			
				if(*oper == address)
				 myMapTemp[i->getOpcodeName()]++;
			 }
		    }
		      ///////////////////////////////////
			 myMap[i->getOpcodeName()]++;

		//	if(address == i->getOperand(0))
		//	 myMapTemp[i->getOpcodeName()]++;
	    	}
	      }
	    }//end of basicblock iterator
	  }//end of function iterator
	}//end of function argument iterator
	   std::map<std::string,int>::iterator itr = myMap.begin();
	   std::map<std::string,int>::iterator itr2 = myMapTemp.begin();

	   while(itr != myMap.end())
	   {
	     errs()<<itr->first <<" is used "<<itr->second<< "times"<<"\n";
	     itr++;
	   }
	   myMap.clear();

	   while(itr2 != myMapTemp.end())
	   {
	     errs()<<itr2->first <<" is used "<<itr2->second<< "times"<<"\n";
	     itr2++;
	   }
	   myMapTemp.clear();

	 return false;
    } //end of runOnFunction
  }; //end of Class 
} //end of llvm namesoace

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
