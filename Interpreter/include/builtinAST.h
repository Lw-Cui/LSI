#ifndef LSI_BUILTINAST_H
#define LSI_BUILTINAST_H

#include <AST.h>

namespace ast {
    class BuiltinConsAST : public ExprAST {
    public:
        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinCarAST : public ExprAST {
    public:

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinCdrAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinAddAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinMultiplyAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinListAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinNullAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinLessThanAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinOppositeAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };

    class BuiltinReciprocalAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr apply(const std::vector<pExpr> &&actualArgs, pScope &) override;

        pExpr getPointer() const override;
    };


    class BuiltinDrawAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;

        using pExpr = std::shared_ptr<ExprAST>;
    };
}
#endif //LSI_BUILTINAST_H
