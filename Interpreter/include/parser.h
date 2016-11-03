
#ifndef GI_PARSER_H
#define GI_PARSER_H

#include <memory>
#include <string>
#include <map>
#include <lexers.h>

namespace parser {
    class ExprAST;

    typedef std::shared_ptr<ExprAST> ASTPtr;
    typedef std::map<std::string, ASTPtr> Scope;

    class ExprAST {
    public:
        virtual ASTPtr eval(Scope &) const = 0;

        virtual ~ExprAST() {}
    };

    class NumberAST : public ExprAST {
    public:
        NumberAST(double n) : value{n} {}

        double getValue() const { return value; }

        ASTPtr eval(Scope &) const override {
            return std::make_shared<NumberAST>(getValue());
        }

    private:
        double value;
    };

    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string &tid) : id{tid} {}

        std::string getId() const { return id; }

        ASTPtr eval(Scope &ss) const override {
            if (ss.count(getId()))
                return ss[getId()]->eval(ss);
            else
                return std::make_shared<IdentifierAST>(getId());
        }

    private:
        std::string id;
    };

    class FunctionDefine : public ExprAST {
    public:
    private:
    };

    class IdentifierDefine : public ExprAST {
    public:
        ASTPtr eval(Scope &ss) const override {
            auto idPtr = dynamic_cast<parser::IdentifierAST *>(identifier.get());
            if (idPtr)
                ss[idPtr->getId()] = value->eval(ss);
            else
                throw std::logic_error("Identifier Define error.");
        }

        IdentifierDefine(ASTPtr id, ASTPtr v) : identifier{id}, value{v} {}

    private:
        ASTPtr identifier, value;
    };

    class FunctionCall : public ExprAST {
    public:
    private:

    };

    ASTPtr parseExpr(lexers::Lexer &lex);

    ASTPtr parseNumberExpr(lexers::Lexer &lex);

    ASTPtr parseIdentifierExpr(lexers::Lexer &lex);

    ASTPtr parseFunctionCallExpr(lexers::Lexer &lex);

    ASTPtr parseLetExpr(lexers::Lexer &lex);

    ASTPtr parseDefinitionExpr(lexers::Lexer &lex);

    ASTPtr parseIdDefinitionExpr(lexers::Lexer &lex);

    ASTPtr parseFunctionDefinitionExpr(lexers::Lexer &lex);
}
#endif //GI_PARSER_H
