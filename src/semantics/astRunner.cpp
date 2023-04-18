//
// Created by Adrian Casares on 4/17/23.
//
// function to run the AST

#include <cmath>
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

  void* ASTRunner::runASTNode(Snapp::SyntaxNode *node) {
    if(node == nullptr) {
      return nullptr;
    }

    if (auto literal = dynamic_cast<SyntaxNodeLiteral*>(node)) {
      return &literal->value;
    }

    if (auto identifier = dynamic_cast<SyntaxNodeIdentifier*>(node)) {
//      std::cout << "Identifier: " << identifier->output() << std::endl;
    }

    if (auto unaryExpression = dynamic_cast<SyntaxNodeUnaryExpression*>(node)) {
//      std::cout << "Unary Expression: " << unaryExpression->output() << std::endl;
//      runASTNode(unaryExpression->operand);
    }

    if (auto binaryExpression = dynamic_cast<SyntaxNodeBinaryExpression*>(node)) {
      if(binaryExpression->operation == Operation::Exponent) {
        float* left = (float*) runASTNode(binaryExpression->leftSide);
        float* right = (float*) runASTNode(binaryExpression->rightSide);

        float* result = new float;
        *result = pow(*left, *right);

        return result;
      }

      if(binaryExpression->operation == Operation::Multiply) {
        float* left = (float*) runASTNode(binaryExpression->leftSide);
        float* right = (float*) runASTNode(binaryExpression->rightSide);

        float* result = new float;
        *result = *left * *right;

        return result;
      }

      if(binaryExpression->operation == Operation::Divide) {
          float* left = (float*) runASTNode(binaryExpression->leftSide);
          float* right = (float*) runASTNode(binaryExpression->rightSide);

          float* result = new float;
          *result = *left / *right;

          return result;
      }

      if(binaryExpression->operation == Operation::Modulus) {
          float* left = (float*) runASTNode(binaryExpression->leftSide);
          float* right = (float*) runASTNode(binaryExpression->rightSide);

          float* result = new float;
          *result = fmod(*left, *right);

          return result;
      }

      if(binaryExpression->operation == Operation::Add) {
          std::variant<std::string, int, float>* left = static_cast<std::variant<std::string, int, float>*>(runASTNode(binaryExpression->leftSide));
          std::variant<std::string, int, float>* right = static_cast<std::variant<std::string, int, float>*>(runASTNode(binaryExpression->rightSide));


          if (std::holds_alternative<int>(*left) && std::holds_alternative<int>(*right)) {
            std::cout << "Adding two ints" << std::endl;
              int* result = new int;
              *result = std::get<int>(*left) + std::get<int>(*right);
              return result;
          }

          if (std::holds_alternative<float>(*left) && std::holds_alternative<float>(*right)) {
            std::cout << "Adding two floats" << std::endl;
              float* result = new float;
              *result = std::get<float>(*left) + std::get<float>(*right);
              return result;
          }

          if (std::holds_alternative<float>(*left) && std::holds_alternative<int>(*right)) {
            std::cout << "Adding a float and an int" << std::endl;
              float* result = new float;
              *result = std::get<float>(*left) + std::get<int>(*right);
              return result;
          }

          if (std::holds_alternative<int>(*left) && std::holds_alternative<float>(*right)) {
            std::cout << "Adding an int and a float" << std::endl;
              float* result = new float;
              *result = std::get<int>(*left) + std::get<float>(*right);
              return result;
          }

          if (std::holds_alternative<std::string>(*left) && std::holds_alternative<std::string>(*right)) {
            std::cout << "Adding two strings" << std::endl;
              std::string* result = new std::string;
              *result = std::get<std::string>(*left) + std::get<std::string>(*right);
              return result;
          }

          std::cout << "Error: Cannot add these types" << std::endl;
      }

      if(binaryExpression->operation == Operation::Subtract) {
          float* left = (float*) runASTNode(binaryExpression->leftSide);
          float* right = (float*) runASTNode(binaryExpression->rightSide);

          float* result = new float;
          *result = *left - *right;

          return result;
      }

      if(binaryExpression->operation == Operation::LessThan) {
          float* left = (float*) runASTNode(binaryExpression->leftSide);
          float* right = (float*) runASTNode(binaryExpression->rightSide);

          bool* result = new bool;
          *result = *left < *right;

          return result;
      }

      if(binaryExpression->operation == Operation::GreaterThan) {
          float* left = (float*) runASTNode(binaryExpression->leftSide);
          float* right = (float*) runASTNode(binaryExpression->rightSide);

          bool* result = new bool;
          *result = *left > *right;

          return result;
      }

        if(binaryExpression->operation == Operation::LessEqual) {
            float* left = (float*) runASTNode(binaryExpression->leftSide);
            float* right = (float*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left <= *right;

            return result;
        }

        if(binaryExpression->operation == Operation::GreaterEqual) {
            float* left = (float*) runASTNode(binaryExpression->leftSide);
            float* right = (float*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left >= *right;

            return result;
        }

        if(binaryExpression->operation == Operation::Equal) {
            float* left = (float*) runASTNode(binaryExpression->leftSide);
            float* right = (float*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left == *right;

            return result;
        }

        if(binaryExpression->operation == Operation::NotEqual) {
            float* left = (float*) runASTNode(binaryExpression->leftSide);
            float* right = (float*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left != *right;

            return result;
        }

        // Bitwise operations
        if(binaryExpression->operation == Operation::BitwiseAnd) {
            int* left = (int*) runASTNode(binaryExpression->leftSide);
            int* right = (int*) runASTNode(binaryExpression->rightSide);

            int* result = new int;
            *result = *left & *right;

            return result;
        }

        if(binaryExpression->operation == Operation::BitwiseXor) {
          int* left = (int*) runASTNode(binaryExpression->leftSide);
          int* right = (int*) runASTNode(binaryExpression->rightSide);

          int* result = new int;
          *result = *left ^ *right;

          return result;
        }

        if(binaryExpression->operation == Operation::BitwiseOr) {
            int* left = (int*) runASTNode(binaryExpression->leftSide);
            int* right = (int*) runASTNode(binaryExpression->rightSide);

            int* result = new int;
            *result = *left | *right;

            return result;
        }

        // Logical operations

        if(binaryExpression->operation == Operation::And) {
            bool* left = (bool*) runASTNode(binaryExpression->leftSide);
            bool* right = (bool*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left && *right;

            return result;
        }

        if(binaryExpression->operation == Operation::Or) {
            bool* left = (bool*) runASTNode(binaryExpression->leftSide);
            bool* right = (bool*) runASTNode(binaryExpression->rightSide);

            bool* result = new bool;
            *result = *left || *right;

            return result;
        }

        if(binaryExpression->operation == Operation::Assign) {
            float* left = (float*) runASTNode(binaryExpression->leftSide);
            float* right = (float*) runASTNode(binaryExpression->rightSide);

            *left = *right;

            return left;
        }

    }

    if (auto functionCall = dynamic_cast<SyntaxNodeFunctionCall*>(node)) {
//      std::cout << "Function Call: " << functionCall->output() << std::endl;
//      runASTNode(functionCall->callee);
//      for (auto &argument : functionCall->arguments) {
//        runASTNode(argument);
//      }
    }

    if (auto variableDeclaration = dynamic_cast<SyntaxNodeVariableDeclaration*>(node)) {
      void* v = runASTNode(variableDeclaration->value);

      if(variableDeclaration->dataType.base() == BaseDataType::Int) {
          int* value = new int;

          if(v == nullptr) {
              *value = 0;
          } else {
              *value = *((int*) v);
            std::cout << "Value: " << *value << std::endl;
          }

          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Float) {
          float* value = new float;

          if(v == nullptr) {
              *value = 0.0;
          } else {
              *value = *((float*) v);
              std::cout << "Value: " << *value << std::endl;
          }

          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Bool) {
          bool* value = new bool;

          if(v == nullptr) {
              *value = false;
          } else {
              *value = *((bool*) v);
          }

          addIdentifier(variableDeclaration->identifier->name, value);
      }

      if(variableDeclaration->dataType.base() == BaseDataType::Str) {
          std::string* value = new std::string;

          if(v == nullptr) {
              *value = "";
          } else {
              *value = *((std::string*) v);
          }

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
