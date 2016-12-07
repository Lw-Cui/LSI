#ifndef GI_AST_H
#define GI_AST_H

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <easylogging++.h>
#include <context.h>

namespace ast {
    using context::Scope;

    class ExprAST {
    public:
        virtual std::shared_ptr<ExprAST> eval(Scope &) const;

        virtual std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &, Scope &);

        virtual std::string display() const;

        virtual ~ExprAST() {}
    };

    class AllExprAST : public ExprAST {
    public:
        AllExprAST(const std::vector<std::shared_ptr<ExprAST>> &v) : exprVec{v} {}

        virtual std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::vector<std::shared_ptr<ExprAST>> exprVec;
    };

    class BooleansFalseAST : public ExprAST {
    public:
        BooleansFalseAST() {}

        virtual std::shared_ptr<ExprAST> eval(Scope &) const override;

        virtual std::string display() const override;
    };

    class BooleansTrueAST : public ExprAST {
    public:
        BooleansTrueAST() {}

        virtual std::shared_ptr<ExprAST> eval(Scope &) const override;

        virtual std::string display() const override;
    };

    class NumberAST : public ExprAST {
    public:
        NumberAST(double n) : value{n} {}

        double getValue() const { return value; }

        std::shared_ptr<ExprAST> eval(Scope &) const override;

        virtual std::string display() const override;

    private:
        double value;
    };

    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string &tid) : id{tid} {}

        std::string getId() const { return id; }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::string id;
    };

    class IfStatementAST : public ExprAST {
    public:
        IfStatementAST(std::shared_ptr<ExprAST> c, std::shared_ptr<ExprAST> t, std::shared_ptr<ExprAST> f) :
                condition{c}, trueClause{t}, falseClause{f} {}

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::shared_ptr<ExprAST> condition;
        std::shared_ptr<ExprAST> trueClause, falseClause;
    };

    class CondStatementAST : public ExprAST {
    public:
        CondStatementAST(const std::vector<std::shared_ptr<ExprAST>> &, const std::vector<std::shared_ptr<ExprAST>> &);

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::shared_ptr<ExprAST> ifStatement;
    };

    class LetStatementAST : public ExprAST {
    public:
        LetStatementAST(const std::vector<std::shared_ptr<ExprAST>> &id, const std::vector<std::shared_ptr<ExprAST>> &v,
                        const std::shared_ptr<ExprAST> &e) : identifier{id}, value{v}, expr{e} {}

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::vector<std::shared_ptr<ExprAST>> identifier, value;
        std::shared_ptr<ExprAST> expr;
    };

    class LoadingFileAST : public ExprAST {
    public:
        LoadingFileAST(const std::string &f) : filename{f} {}

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::string filename;
    };


    class PairAST : public ExprAST {
    public:
        PairAST(std::shared_ptr<ExprAST> f, std::shared_ptr<ExprAST> s) : data{f, s} {}

        std::shared_ptr<ExprAST> eval(Scope &s) const override;

        virtual std::string display() const override;

        std::pair<std::shared_ptr<ExprAST>, std::shared_ptr<ExprAST>> data;

    };

    class NilAST : public ExprAST {
    public:
        NilAST() {}

        std::shared_ptr<ExprAST> eval(Scope &s) const override;

        virtual std::string display() const override;
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

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<ExprAST> value;
    };

    class LambdaAST : public ExprAST {
    public:
        LambdaAST(const std::vector<std::string> &v,
                  std::shared_ptr<ExprAST> expr,
                  const std::vector<std::shared_ptr<ExprAST>> &nested)
                : formalArgs{v}, expression{expr}, nestedFunc{nested} {}

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

        virtual std::string display() const override;

        Scope &upgradeScope() const { return context; }

    private:
        std::vector<std::string> formalArgs;
        std::shared_ptr<ExprAST> expression;
        std::vector<std::shared_ptr<ExprAST>> nestedFunc;
        mutable Scope context;
    };

    class BuiltinConsAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinCarAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinCdrAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinAddAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinMultiplyAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinListAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinNullAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinLessThanAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinOppositeAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class BuiltinReciprocalAST : public ExprAST {
    public:
        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;
    };

    class LambdaBindingAST : public BindingAST {
    public:
        LambdaBindingAST(const std::string &id,
                         const std::vector<std::string> &v,
                         std::shared_ptr<ExprAST> expr,
                         const std::vector<std::shared_ptr<ExprAST>> &nested) :
                BindingAST(id), lambda{std::make_shared<LambdaAST>(v, expr, nested)} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<LambdaAST> lambda;
    };

    class LambdaApplicationAST : public ExprAST {
    public:
        LambdaApplicationAST(const std::shared_ptr<ExprAST> &lam, const std::vector<std::shared_ptr<ExprAST>> &args)
                : lambdaOrIdentifier{lam}, actualArgs{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<ExprAST> lambdaOrIdentifier;
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

}

#endif //GI_AST_H
