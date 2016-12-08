#ifndef GI_BUILTINDRAW_H
#define GI_BUILTINDRAW_H

#include <AST.h>
#include <SFML/Graphics.hpp>
#include <Controller.h>

namespace ast {
    class GUIBuiltinAST : public ExprAST {
    protected:
        sf::Vector2f toVec2f(const std::shared_ptr<ExprAST> &) const;

        std::shared_ptr<ExprAST> toPairAST(const sf::Vector2f &) const;
    };

    class BuiltinDrawAST : public GUIBuiltinAST {
    public:
        BuiltinDrawAST(con::Controller &);

        std::string display() const override;

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

    private:

        con::Controller &controller;
    };

    class BuiltinLineAST : public GUIBuiltinAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

    private:
        std::vector<sf::Vector2f> getPointSet(sf::Vector2f begin, sf::Vector2f end) const;

        std::vector<sf::Vector2f> SimpleLinePointSet(sf::Vector2f begin, sf::Vector2f end) const;
    };
}

#endif //GI_BUILTINDRAW_H
