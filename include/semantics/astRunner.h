#ifndef SNAPP_SEMANTICS_ASTRUNNER_H_
#define SNAPP_SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"
#include "semantics/scope.h"
#include "value/dataValue.h"
#include "value/function.h"

#include <optional>
#include <vector>

namespace Snapp {

    class ASTRunner {
    public:
        ASTRunner(bool enableDebug = false);
        ~ASTRunner();

        static Scope* runAST(const AbstractSyntaxTree& ast, bool enableDebug = false, bool keep = false);

        Scope& currentScope();
        Scope& currentStrongScope();

        std::optional<DataValue> runASTNode(const SyntaxNode* node);
     private:
      std::vector<Scope*> scopes_;
      size_t scopeIndex_;
      bool debugEnabled_;
      void setScope(Scope* scope);
      size_t createScope(bool strong, bool isFunction = false, ClassValue* classValue = nullptr);
      std::optional<DataValue> runFunction(const FunctionOverload& function, const SyntaxNodeFunctionCall* call, Scope* scope);
    };

}

#endif /* SNAPP_SEMANTICS_ASTRUNNER_H_ */
