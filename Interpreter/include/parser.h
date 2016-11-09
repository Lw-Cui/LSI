
#ifndef GI_PARSER_H
#define GI_PARSER_H

#include <exception>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <easylogging++.h>
#include <lexers.h>

namespace parser {
    class ExprAST;

    typedef std::map<std::string, std::shared_ptr<ExprAST>> Scope;

    class ExprAST {
    public:
        virtual std::shared_ptr<ExprAST> eval(Scope &) const {
            CLOG(DEBUG, "exception");
            throw std::logic_error("Expression cannot be evaluated.");
        };

        virtual std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &) {
            CLOG(DEBUG, "exception");
            throw std::logic_error("Expression cannot be applied.");
        };

        virtual bool toBool() const { return true; }

        virtual ~ExprAST() {}
    };

    class NumberAST : public ExprAST {
    public:
        NumberAST(double n) : value{n} {}

        double getValue() const { return value; }

        virtual bool toBool() const override { return getValue() != 0; }

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
            CLOG(DEBUG, "parser") << "Evaluate identifier: " << getId();
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

    class LambdaAST : public ExprAST {
    public:
        LambdaAST(const std::vector<std::string> &v,
                  std::shared_ptr<ExprAST> expr) : formalArgs{v}, expression{expr} {}

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs) override {
            Scope s;
            for (size_t i = 0; i < actualArgs.size(); i++)
                s[formalArgs[i]] = actualArgs[i]->eval(s);
            return expression->eval(s);
        }

    protected:
        std::vector<std::string> formalArgs;
        std::shared_ptr<ExprAST> expression;
    };

    class FunctionAST : public LambdaAST {
    public:
        FunctionAST(const std::vector<std::string> &v,
                    std::shared_ptr<ExprAST> expr) : LambdaAST{v, expr} {}

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs) override {
            CLOG(DEBUG, "parser") << "Apply function now. Number of formal arguments is " << formalArgs.size();
            for (size_t i = 0; i < actualArgs.size(); i++) {
                CLOG(DEBUG, "parser") << "Set formal argument: " << formalArgs[i];
                context[formalArgs[i]] = actualArgs[i]->eval(context);
            }
            return expression->eval(context);
        }

        void setContext(const Scope &s) {
            context = s;
        }

    private:
        Scope context;
    };

    class IdentifierBindingAST : public ExprAST {
    public:
        IdentifierBindingAST(const std::string &id) : identifier{id} {}

        const std::string &getIdentifier() const {
            return identifier;
        }

    private:
        std::string identifier;
    };

    class ValueBindingAST : public IdentifierBindingAST {
    public:
        ValueBindingAST(const std::string &id, std::shared_ptr<ExprAST> v)
                : IdentifierBindingAST(id), value{v} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            ss[getIdentifier()] = value->eval(ss);
            return nullptr;
        }

    private:
        std::shared_ptr<ExprAST> value;
    };


    class FunctionBindingAST : public IdentifierBindingAST {
    public:
        FunctionBindingAST(const std::string &id,
                           const std::vector<std::string> &v,
                           std::shared_ptr<ExprAST> expr) :
                IdentifierBindingAST(id), func{std::make_shared<FunctionAST>(v, expr)} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            func->setContext(ss);
            ss[getIdentifier()] = func;
            return nullptr;
        }

    private:
        std::shared_ptr<FunctionAST> func;
    };

    class LambdaCallAST : public ExprAST {
    public:
        LambdaCallAST(const std::shared_ptr<ExprAST> &lam, const std::vector<std::shared_ptr<ExprAST>> &args)
                : lambda{lam}, actualArgs{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            return lambda->apply(actualArgs);
        }

    private:
        std::shared_ptr<ExprAST> lambda;
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    class FunctionCallAST : public ExprAST {
    public:
        FunctionCallAST(std::string id, const std::vector<std::shared_ptr<ExprAST>> &args)
                : identifier{id}, actualArgs{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            CLOG(DEBUG, "parser") << "Apply function call. Number of actual arguments: " << actualArgs.size();
            if (ss.count(identifier)) {
                return ss[identifier]->apply(actualArgs);
            } else {
                CLOG(DEBUG, "exception");
                throw std::logic_error("Unbound function identifier.");
            }
        }

    private:
        std::string identifier;
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    std::shared_ptr<ExprAST> parseExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseNumberExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdentifierExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLambdaCallExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionCallExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLetExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionDefinitionExpr(lexers::Lexer &lex);
}
#endif //GI_PARSER_H
