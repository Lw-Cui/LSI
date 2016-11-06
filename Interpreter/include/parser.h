
#ifndef GI_PARSER_H
#define GI_PARSER_H

#include <memory>
#include <string>
#include <map>
#include <lexers.h>

namespace parser {
    class ExprAST;

    typedef std::map<std::string, std::shared_ptr<ExprAST>> Scope;

    class ExprAST {
    public:
        virtual std::shared_ptr<ExprAST> eval(Scope &) const = 0;

        virtual ~ExprAST() {}
    };

    class NumberAST : public ExprAST {
    public:
        NumberAST(double n) : value{n} {}

        double getValue() const { return value; }

        std::shared_ptr<ExprAST> eval(Scope &) const override {
            return std::make_shared<NumberAST>(getValue());
        }

    private:
        double value;
    };

    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string &tid) : id{tid} {}

        std::string getId() const { return id; }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            if (ss.count(getId())) {
                return ss[getId()]->eval(ss);
            } else {
                CLOG(DEBUG, "exception");
                throw std::logic_error("Unbound identifier.");
            }
        }

    private:
        std::string id;
    };

    class FunctionDefinitionAST : public ExprAST {
    public:
    private:
    };

    class IdentifierDefinitionAST : public ExprAST {
    public:
        IdentifierDefinitionAST(std::shared_ptr<IdentifierAST> id,
                                std::shared_ptr<ExprAST> v) : identifier{id}, value{v} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            ss[identifier->getId()] = value->eval(ss);
            return nullptr;
        }

    private:
        std::shared_ptr<IdentifierAST> identifier;
        std::shared_ptr<ExprAST> value;
    };

    class FunctionCall : public ExprAST {
    public:
    private:
    };


    std::shared_ptr<ExprAST> parseExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseNumberExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdentifierExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionCallExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLetExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionDefinitionExpr(lexers::Lexer &lex);
}
#endif //GI_PARSER_H
