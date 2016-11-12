
#ifndef GI_PARSER_H
#define GI_PARSER_H

#include <exception>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <easylogging++.h>
#include <lexers.h>
#include <context.h>

namespace parser {
    using context::Scope;

    class ExprAST {
    public:
        virtual std::shared_ptr<ExprAST> eval(Scope &) const {
            CLOG(DEBUG, "exception");
            throw std::logic_error("Expression cannot be evaluated.");
        };

        virtual std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &, const Scope &) {
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

    class ArgumentsAST : public ExprAST {
    public:
        ArgumentsAST(const std::vector<std::string> &v) : formalArgs{v} {}

        void bindArguments(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
            CLOG(DEBUG, "parser") << "Number of formal arguments is " << formalArgs.size();
            for (size_t i = 0; i < actualArgs.size(); i++) {
                CLOG(DEBUG, "parser") << "Set formal argument: " << formalArgs[i];
                s[formalArgs[i]] = actualArgs[i]->eval(s);
            }
        }

    protected:
        std::vector<std::string> formalArgs;
    };

    class LambdaAST : public ArgumentsAST {
    public:
        LambdaAST(const std::vector<std::string> &v,
                  std::shared_ptr<ExprAST> expr) : ArgumentsAST{v}, expression{expr} {}

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs,
                                       const Scope &) override {
            bindArguments(actualArgs, context);
            return expression->eval(context);
        }

        void setContext(const Scope &s) {
            context = s;
        }

    protected:
        std::shared_ptr<ExprAST> expression;
        Scope context;
    };

    class OperatorAST : public ExprAST {
    public:
        OperatorAST(const std::vector<std::shared_ptr<ExprAST>> &v) : actualArgs{v} {}

    protected:
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    class AddOperatorAST : public OperatorAST {
    public:
        AddOperatorAST(const std::vector<std::shared_ptr<ExprAST>> &v) : OperatorAST{v} {}

        std::shared_ptr<ExprAST> eval(Scope &s) const override {
            double num = 0;
            for (auto element: actualArgs) {
                std::shared_ptr<ExprAST> res = element->eval(s);
                if (NumberAST *p = dynamic_cast<NumberAST *>(res.get())) {
                    CLOG(DEBUG, "parser") << "Add number: " << p->getValue();
                    num += p->getValue();
                } else {
                    CLOG(DEBUG, "exception");
                    throw std::logic_error("The operands cannot be converted to number");
                }
            }
            return std::make_shared<NumberAST>(num);

        }

    };

    class BindingAST : public ExprAST {
    public:
        BindingAST(const std::string &id) : identifier{id} {}

        const std::string &getIdentifier() const {
            return identifier;
        }

    private:
        std::string identifier;
    };

    class ValueBindingAST : public BindingAST {
    public:
        ValueBindingAST(const std::string &id, std::shared_ptr<ExprAST> v)
                : BindingAST(id), value{v} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            ss[getIdentifier()] = value->eval(ss);
            return nullptr;
        }

    private:
        std::shared_ptr<ExprAST> value;
    };


    class LambdaBindingAST : public BindingAST {
    public:
        LambdaBindingAST(const std::string &id,
                         const std::vector<std::string> &v,
                         std::shared_ptr<ExprAST> expr) :
                BindingAST(id), lambda{std::make_shared<LambdaAST>(v, expr)} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            lambda->setContext(ss);
            ss[getIdentifier()] = lambda;
            return nullptr;
        }

    private:
        std::shared_ptr<LambdaAST> lambda;
    };

    class ApplicationAST : public ExprAST {
    public:
        ApplicationAST(const std::vector<std::shared_ptr<ExprAST>> &v) : actualArgs{v} {}

    protected:
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    class LambdaApplicationAST : public ApplicationAST {
    public:
        LambdaApplicationAST(const std::shared_ptr<ExprAST> &lam, const std::vector<std::shared_ptr<ExprAST>> &args)
                : lambda{lam}, ApplicationAST{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            return lambda->apply(actualArgs, ss);
        }

    private:
        std::shared_ptr<ExprAST> lambda;
    };

    class FunctionApplicationAST : public ApplicationAST {
    public:
        FunctionApplicationAST(std::string id, const std::vector<std::shared_ptr<ExprAST>> &args)
                : identifier{id}, ApplicationAST{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override {
            CLOG(DEBUG, "parser") << "Apply function call. Number of actual arguments: " << actualArgs.size();
            if (ss.count(identifier)) {
                return ss[identifier]->apply(actualArgs, ss);
            } else {
                CLOG(DEBUG, "exception");
                throw std::logic_error("Unbound function identifier.");
            }
        }

    private:
        std::string identifier;
    };

    std::shared_ptr<ExprAST> parseExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseNumberExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseOperatorExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseAddOperatorExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdentifierExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLambdaCallExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionCallExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLetExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionDefinitionExpr(lexers::Lexer &lex);
}
#endif //GI_PARSER_H
