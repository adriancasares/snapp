#ifndef SEMANTICS_ASTRUNNER_H_
#define SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"
#include "semantics/scope.h"

#include <optional>

namespace Snapp {

    class ASTRunner {
    public:
        static void runAST(const AbstractSyntaxTree& ast, const bool isDebug = false);
        ASTRunner(const bool isDebug = false);
        ~ASTRunner();

        Scope& currentScope();

    private:
        std::vector<Scope*> scopes_;
        int scopeIndex_;
        bool isDebug_;

        int createScope(bool isFunction = false);
        std::optional<DataValue> runASTNode(const SyntaxNode* node);
        std::optional<DataValue> runFunction(const std::variant<FunctionValue, NativeFunctionValue>& function, const SyntaxNodeFunctionCall* call);
//        std::optional<R_Value> getRValue(const SyntaxNode* node);
    };

}

#endif /* SEMANTICS_ASTRUNNER_H_ */
