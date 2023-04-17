//
// Created by Adrian Casares on 4/17/23.
//

#ifndef SNAPP_INCLUDE_SEMANTICS_ASTRUNNER_H_
#define SNAPP_INCLUDE_SEMANTICS_ASTRUNNER_H_

#include "../syntax/abstractSyntaxTree.h"

namespace Snapp {

    class ASTRunner {
      private:
        ASTRunner() = default;
        static std::map<std::string, SyntaxNode*> identifiers;
        void addIdentifier(SyntaxNodeIdentifier* identifier, SyntaxNode* value);
        void runASTNode(SyntaxNode* node);

      public:
        static void runAST(AbstractSyntaxTree& ast);
    };

}
#endif //SNAPP_INCLUDE_SEMANTICS_ASTRUNNER_H_
