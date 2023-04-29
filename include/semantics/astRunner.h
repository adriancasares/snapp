#ifndef SEMANTICS_ASTRUNNER_H_
#define SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"
#include "semantics/scope.h"

#include <optional>

namespace Snapp {

    class ASTRunner {
    public:
        static void runAST(const AbstractSyntaxTree& ast);
        ASTRunner();
        ~ASTRunner();

        Scope& currentScope();

    private:
        std::vector<Scope*> scopes_;
        int scopeIndex_;

        int createScope(bool isFunction = false);
        std::optional<DataValue> runASTNode(const SyntaxNode* node);
    };

}

#endif /* SEMANTICS_ASTRUNNER_H_ */
