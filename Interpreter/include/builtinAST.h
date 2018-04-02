#ifndef LSI_BUILTINAST_H
#define LSI_BUILTINAST_H

#include <AST.h>

namespace ast {
    class BuiltinConsAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinCarAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinCdrAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinAddAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinMultiplyAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinListAST : public ExprAST {
    public:

        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinNullAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinLessThanAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinOppositeAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };

    class BuiltinReciprocalAST : public ExprAST {
    public:
        APPLY_FUNC

        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };


    class BuiltinDrawAST : public ExprAST {
    public:
        void accept(visitor::NodeVisitor &visitor) const override;

        pExpr getPointer() const override;
    };
}
#endif //LSI_BUILTINAST_H
