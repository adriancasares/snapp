#ifndef SNAPP_SEMANTICS_ASTRUNNER_H_
#define SNAPP_SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"
#include "semantics/scope.h"
#include "value/dataValue.h"
#include "value/function.h"

#include <optional>
#include <vector>

namespace Snapp {

    std::optional<FloatValue> coerceFloat(DataValue value);
    std::optional<BoolValue> coerceBool(DataValue value);
    std::optional<StrValue> coerceStr(DataValue value);
    std::optional<IntValue> coerceInt(DataValue value);

    class ASTRunner {
    public:
        ASTRunner(bool enableDebug = false);
        ~ASTRunner();

        static void runAST(const AbstractSyntaxTree& ast, bool enableDebug = false);

        Scope& currentScope();

    private:
        std::vector<Scope*> scopes_;
        size_t scopeIndex_;
        bool debugEnabled_;

        size_t createScope(bool isFunction = false, bool isClass = false);
        std::optional<DataValue> runASTNode(const SyntaxNode* node);
        std::optional<DataValue> runFunction(const SimpleFunctionValue& function, const SyntaxNodeFunctionCall* call);
    };

}

#endif /* SNAPP_SEMANTICS_ASTRUNNER_H_ */
