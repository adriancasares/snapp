//
// Created by Adrian Casares on 4/17/23.
//
// function to run the AST

#include "semantics/astRunner.h"

namespace Snapp {

  std::map<std::string, void*> ASTRunner::identifiers;

  void ASTRunner::addIdentifier(std::string identifier, void *value) {
      // check if identifier already exists
        if (identifiers.find(identifier) != identifiers.end()) {
            std::cout << "Identifier already exists: " << identifier << std::endl;
        } else {
            identifiers[identifier] = value;
            std::cout << "Added identifier: " << identifier << std::endl;
        }
  }

  void ASTRunner::runASTNode(Snapp::SyntaxNode *node) {
    if(node == nullptr) {
      return;
    }

    if (auto literal = dynamic_cast<SyntaxNodeLiteral*>(node)) {
//      std::cout << "Literal: " << literal->output() << std::endl;
    }

    if (auto identifier = dynamic_cast<SyntaxNodeIdentifier*>(node)) {
//      std::cout << "Identifier: " << identifier->output() << std::endl;
    }

    if (auto unaryExpression = dynamic_cast<SyntaxNodeUnaryExpression*>(node)) {
//      std::cout << "Unary Expression: " << unaryExpression->output() << std::endl;
//      runASTNode(unaryExpression->operand);
    }

    if (auto binaryExpression = dynamic_cast<SyntaxNodeBinaryExpression*>(node)) {
//      std::cout << "Binary Expression: " << binaryExpression->output() << std::endl;
//      runASTNode(binaryExpression->leftSide);
//      runASTNode(binaryExpression->rightSide);
    }

    if (auto functionCall = dynamic_cast<SyntaxNodeFunctionCall*>(node)) {
//      std::cout << "Function Call: " << functionCall->output() << std::endl;
//      runASTNode(functionCall->callee);
//      for (auto &argument : functionCall->arguments) {
//        runASTNode(argument);
//      }
    }

    if (auto variableDeclaration = dynamic_cast<SyntaxNodeVariableDeclaration*>(node)) {
      if(variableDeclaration->dataType.base() == BaseDataType::Int) {
          int* value = new int;
          *value = 0;
          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Float) {
          float* value = new float;
          *value = 0.0;

          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Bool) {
          bool* value = new bool;
          *value = false;

          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Str) {
          std::string* value = new std::string;
          *value = "";

          addIdentifier(variableDeclaration->identifier->name, value);
      }


      if(variableDeclaration->dataType.base() == BaseDataType::Object) {
            std::cout << "Cannot declare object variable" << std::endl;
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Void) {
          std::cout << "Cannot declare void variable" << std::endl;
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Unknown) {
          std::cout << "Cannot declare unknown variable" << std::endl;
      }
    }

    if(auto blockStatement = dynamic_cast<SyntaxNodeBlockStatement*>(node)) {
//      std::cout << "Block Statement: " << blockStatement->output() << std::endl;
//      for (auto &statement : blockStatement->statements) {
//        runASTNode(statement);
//      }
    }

    if(auto ifStatement = dynamic_cast<SyntaxNodeIfStatement*>(node)) {
//      std::cout << "If Statement: " << ifStatement->output() << std::endl;
//      runASTNode(ifStatement->condition);
//      runASTNode(ifStatement->consequent);
//      runASTNode(ifStatement->alternative);
    }

    if(auto whileStatement = dynamic_cast<SyntaxNodeWhileStatement*>(node)) {
//      std::cout << "While Statement: " << whileStatement->output() << std::endl;
//      runASTNode(whileStatement->condition);
//      runASTNode(whileStatement->body);
    }

    if(auto forStatement = dynamic_cast<SyntaxNodeForStatement*>(node)) {
//      std::cout << "For Statement: " << forStatement->output() << std::endl;
//      runASTNode(forStatement->initialization);
//      runASTNode(forStatement->condition);
//      runASTNode(forStatement->increment);
//      runASTNode(forStatement->body);
    }

    if(auto returnStatement = dynamic_cast<SyntaxNodeReturnStatement*>(node)) {
//      std::cout << "Return Statement: " << returnStatement->output() << std::endl;
//      runASTNode(returnStatement->value);
    };

    if (auto functionDeclaration = dynamic_cast<SyntaxNodeFunctionDeclaration*>(node)) {
      addIdentifier(functionDeclaration->identifier->name, functionDeclaration);
    }

    if (auto classDeclaration = dynamic_cast<SyntaxNodeClassDeclaration*>(node)) {
      addIdentifier(classDeclaration->identifier->name, classDeclaration);
    }

    if(auto observerDeclaration = dynamic_cast<SyntaxNodeObserverDeclaration*>(node)) {
//      std::cout << "Observer Declaration: " << observerDeclaration->output() << std::endl;
//      runASTNode(observerDeclaration->identifier);
//      for (auto &argument : observerDeclaration->arguments) {
//        runASTNode(argument);
//      }
    }
  }

  void ASTRunner::runAST(AbstractSyntaxTree &ast) {
    for (auto &node : ast.root()) {
      ASTRunner *runner = new ASTRunner();
      runner->runASTNode(node);

      delete runner;
    }
  }
}
