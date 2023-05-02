#ifndef SNAPP_SEMANTICS_ASTRUNNER_H_
#define SNAPP_SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"
#include "semantics/scope.h"

#include <optional>

namespace Snapp {

    std::optional<FloatValue> coerceFloat(DataValue value);
    std::optional<BoolValue> coerceBool(DataValue value);
    std::optional<StrValue> coerceStr(DataValue value);

    class ASTRunner {
    public:
        static void runAST(const AbstractSyntaxTree& ast, bool enableDebug = false);
        ASTRunner(bool enableDebug = false);
        ~ASTRunner();

        Scope& currentScope();

    private:
        std::vector<Scope*> scopes_;
        size_t scopeIndex_;
        bool debugEnabled_;

        size_t createScope(bool isFunction = false);
        std::optional<DataValue> runASTNode(const SyntaxNode* node);
    };

}

#endif /* SNAPP_SEMANTICS_ASTRUNNER_H_ */
