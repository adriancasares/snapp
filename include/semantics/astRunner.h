#ifndef SEMANTICS_ASTRUNNER_H_
#define SEMANTICS_ASTRUNNER_H_

#include "syntax/abstractSyntaxTree.h"

#include <optional>

namespace Snapp {

    class ASTRunner {
    private:
        static std::map<std::string, DataValue> identifiers;
        void addIdentifier(std::string identifier, DataValue value);
        std::optional<DataValue> runASTNode(const SyntaxNode* node);

    public:
        ASTRunner() = default;
        static void runAST(const AbstractSyntaxTree& ast);
    };

}

#endif /* SEMANTICS_ASTRUNNER_H_ */
