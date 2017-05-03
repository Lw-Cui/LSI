#ifndef CLI_BUILTINDRAW_H
#define CLI_BUILTINDRAW_H

#include <memory>
#include <AST.h>
#include <image.h>

namespace ast {
    class CLIBuiltinDrawImpl;

    class CLIBuiltinDrawAST : public BuiltinDrawAST {
    public:
        CLIBuiltinDrawAST(Image &i);

        std::string display() const override;

        pExpr apply(const std::vector<pExpr> &actualArgs, pScope &) override;

    private:
        std::pair<float, float> toPair(const std::shared_ptr<ExprAST> &);

        Image &image;
    };
}

#endif //GI_BUILTINDRAW_H
