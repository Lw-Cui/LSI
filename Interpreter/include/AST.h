#ifndef GI_AST_H
#define GI_AST_H

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <easylogging++.h>

namespace visitor {
    class NodeVisitor;
}

namespace context {
    class Scope;
}

namespace ast {
    using context::Scope;

    using pScope = std::shared_ptr<Scope>;

    class ExprAST;

    using pExpr = std::shared_ptr<ExprAST>;

    class ExprAST {
    public:
        // Eval this AST node and return result. The node itself does not be changed.
        virtual pExpr eval(pScope &) const;

        virtual pExpr getPointer() const;

        virtual pExpr apply(std::vector<pExpr> &&, pScope &) const;

        virtual void accept(visitor::NodeVisitor &) const;
    };


    class AllExprAST : public ExprAST {
    public:
        explicit AllExprAST(std::vector<std::shared_ptr<ExprAST>> v) : exprVec{std::move(v)} {}

        void accept(visitor::NodeVisitor &) const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &) const override;

        pExpr getPointer() const override;

    private:
        std::vector<std::shared_ptr<ExprAST>> exprVec;
    };

    class BooleansFalseAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BooleansTrueAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class NumberAST : public ExprAST {
    public:
        explicit NumberAST(double n) : value{n} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        double getValue() const { return value; }

        pExpr getPointer() const override;

    private:
        double value;
    };

    class IdentifierAST : public ExprAST {
    public:
        explicit IdentifierAST(std::string tid) : id{std::move(tid)} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        std::string getId() const { return id; }

        pExpr getPointer() const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &ss) const override;

    private:
        std::string id;
    };

    // deal with anonymous lambda invocation directly and normal function call
    class InvocationAST : public ExprAST {
        friend class IfStatementAST;

    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        InvocationAST(const std::shared_ptr<ExprAST> &lam,
                      const std::vector<std::shared_ptr<ExprAST>> &args);

        pExpr getPointer() const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &ss) const override;

    private:
        std::shared_ptr<ExprAST> callableObj;
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    class IfStatementAST : public ExprAST {
    public:
        IfStatementAST(const std::shared_ptr<ExprAST> &c,
                       const std::shared_ptr<ExprAST> &t,
                       const std::shared_ptr<ExprAST> &f);

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &) const override;

        pExpr getPointer() const override;

        void accept(visitor::NodeVisitor &visitor) const override;

    private:
        std::shared_ptr<ExprAST> condition;
        std::shared_ptr<ExprAST> trueClause, falseClause;
    };

    class CondStatementAST : public ExprAST {
    public:
        CondStatementAST(const std::vector<std::shared_ptr<ExprAST>> &,
                         const std::vector<std::shared_ptr<ExprAST>> &);

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &) const override;

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;

    private:
        std::shared_ptr<ExprAST> ifStatement;
    };

    class LetStatementAST : public ExprAST {
    public:
        LetStatementAST(std::vector<std::shared_ptr<ExprAST>> id,
                        std::vector<std::shared_ptr<ExprAST>> v,
                        std::shared_ptr<ExprAST> e) :
            identifier{std::move(id)}, value{std::move(v)}, expr{std::move(e)} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &) const override;

        pExpr getPointer() const override;

    private:
        std::vector<std::shared_ptr<ExprAST>> identifier, value;
        std::shared_ptr<ExprAST> expr;
    };

    class LoadingFileAST : public ExprAST {
    public:
        explicit LoadingFileAST(std::string f) : filename{std::move(f)} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &) const override;

        pExpr getPointer() const override;

    private:
        std::string filename;
    };


    class PairAST : public ExprAST {
    public:
        PairAST(const std::shared_ptr<ExprAST> &f,
                const std::shared_ptr<ExprAST> &s) : data{f, s} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &s) const override;

        mutable std::pair<std::shared_ptr<ExprAST>, std::shared_ptr<ExprAST>> data;

        pExpr getPointer() const override;

    };

    class NilAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };


    class BindingAST : public ExprAST {
    public:
        explicit BindingAST(std::string id) : identifier{std::move(id)} {}

        void accept(visitor::NodeVisitor &visitor) const override;

        const std::string &getIdentifier() const {
            return identifier;
        }

        pExpr getPointer() const override;

    private:
        std::string identifier;
    };

    class ValueBindingAST : public BindingAST {
    public:
        ValueBindingAST(const std::string &id,
                        const std::shared_ptr<ExprAST> &v);

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &ss) const override;

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;

    private:
        std::shared_ptr<ExprAST> value;
    };

    class LambdaAST : public ExprAST {
        friend class LambdaBindingAST;

    public:
        LambdaAST(std::vector<std::string> v, std::vector<std::shared_ptr<ExprAST>> expr);

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(std::vector<pExpr> &&actualArgs, pScope &) const override;

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &ss) const override;

        pExpr getPointer() const override;

    private:
        std::vector<std::string> formalArgs;
        std::vector<std::shared_ptr<ExprAST>> expression;
        mutable pScope context;
    };


    class LambdaBindingAST : public BindingAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        LambdaBindingAST(const std::string &id,
                         const std::vector<std::string> &v,
                         const std::vector<std::shared_ptr<ExprAST>> &expr) :
            BindingAST(id), lambda{std::make_shared<LambdaAST>(v, expr)} {}

        std::shared_ptr<ExprAST> eval(std::shared_ptr<Scope> &ss) const override;

        pExpr getPointer() const override;

    private:
        std::shared_ptr<LambdaAST> lambda;
    };
}

#endif //GI_AST_H
