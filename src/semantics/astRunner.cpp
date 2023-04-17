//
// Created by Adrian Casares on 4/17/23.
//
// function to run the AST

#include "semantics/astRunner.h"

namespace Snapp {

  std::map<std::string, SyntaxNode*> ASTRunner::identifiers;

  void ASTRunner::addIdentifier(SyntaxNodeIdentifier *identifier, SyntaxNode *value) {
      // check if identifier already exists
        if (identifiers.find(identifier->output()) != identifiers.end()) {
            std::cout << "Identifier already exists: " << identifier->output() << std::endl;
        } else {
            identifiers[identifier->output()] = value;
            std::cout << "Added identifier: " << identifier->output() << std::endl;
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
//      std::cout << "Variable Declaration: " << variableDeclaration->output() << std::endl;
//      runASTNode(variableDeclaration->identifier);
//      runASTNode(variableDeclaration->value);
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
//      std::cout << "Function Declaration: " << functionDeclaration->output() << std::endl;

      addIdentifier(functionDeclaration->identifier, functionDeclaration);
    }

    if (auto classDeclaration = dynamic_cast<SyntaxNodeClassDeclaration*>(node)) {
//      std::cout << "Class Declaration: " << classDeclaration->output() << std::endl;

      addIdentifier(classDeclaration->identifier, classDeclaration);
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
    // loop through tree root
    for (auto &node : ast.root()) {
      ASTRunner *runner = new ASTRunner();
      runner->runASTNode(node);

      delete runner;
    }
  }
}
