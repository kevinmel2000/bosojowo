#include <stack>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
//#include <llvm/IR/Assembly/PrintModulePass.h>
#include <llvm/IR/IRPrintingPasses.h>
#include "llvm/IR/LegacyPassManager.h"
#include <llvm/IR/IRBuilder.h>
//#include <llvm/IR/ModuleProvider.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class NBlock;
class NVariableDeclaration;

class CodeGenBlock {
public:
    BasicBlock *block;
    std::map<std::string, Value*> locals;
    std::map<std::string, NVariableDeclaration*> localVarDecls;
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;
    Function *mainFunction;
    

public:
    Module *module;
    CodeGenContext();

    void generateCode(NBlock& root);
    GenericValue runCode();
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    std::map<std::string, NVariableDeclaration*>& localVarDecs() { return blocks.top()->localVarDecls; }
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    void copyLocals(std::map<std::string, Value*>& anotherLocals){ blocks.top()->locals = anotherLocals; }

};
