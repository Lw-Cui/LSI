#ifndef CLI_BUILTINDRAW_H
#define CLI_BUILTINDRAW_H

#include <AST.h>

namespace ast {
    class CLIBuiltinDrawAST : public GUIBuiltinDrawAST {
    public:
        BuiltinDrawAST();

        std::string display() const override;

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

    private:

        con::Controller &controller;
    };
}

#endif //GI_BUILTINDRAW_H
