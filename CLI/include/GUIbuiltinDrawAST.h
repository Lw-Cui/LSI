#ifndef CLI_BUILTINDRAW_H
#define CLI_BUILTINDRAW_H

#include <AST.h>

#define cimg_display 0
#include <CImg.h>

namespace ast {
    class CLIBuiltinDrawAST : public BuiltinDrawAST {
    public:
        CLIBuiltinDrawAST(cimg_library::CImg<float> &im);

        std::string display() const override;

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

    private:
        std::pair<float, float> toPair(const std::shared_ptr<ExprAST> &);

        cimg_library::CImg<float> &image;
    };
}

#endif //GI_BUILTINDRAW_H
