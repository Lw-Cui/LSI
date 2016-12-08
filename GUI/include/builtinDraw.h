#ifndef GI_BUILTINDRAW_H
#define GI_BUILTINDRAW_H

#include <AST.h>
#include <SFML/Graphics.hpp>
#include <Controller.h>

namespace ast {
    class BuiltinDrawAST : public ExprAST {
    public:
        BuiltinDrawAST(con::Controller &);

        std::string display() const override;

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

    private:
        sf::Vector2f toVec2f(const std::shared_ptr<ExprAST> &) const;

        con::Controller &controller;
    };
}

#endif //GI_BUILTINDRAW_H
