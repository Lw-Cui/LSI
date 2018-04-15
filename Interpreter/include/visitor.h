#ifndef GI_VISITOR_H
#define GI_VISITOR_H

#include <string>
#include <AST.h>
#include <builtinAST.h>

namespace visitor {
    class NodeVisitor {
    public:
        virtual void visitExprAST(const ast::ExprAST &) {}

        virtual void visitAllExprAST(const ast::AllExprAST &) {}

        virtual void visitBooleansFalseAST(const ast::BooleansFalseAST &) {}

        virtual void visitBooleansTrueAST(const ast::BooleansTrueAST &) {}

        virtual void visitNumberAST(const ast::NumberAST &) {}

        virtual void visitIdentifierAST(const ast::IdentifierAST &) {}

        virtual void visitIfStatementAST(const ast::IfStatementAST &) {}

        virtual void visitCondStatementAST(const ast::CondStatementAST &) {}

        virtual void visitLetStatementAST(const ast::LetStatementAST &) {}

        virtual void visitLoadingFileAST(const ast::LoadingFileAST &) {}

        virtual void visitPairAST(const ast::PairAST &) {}

        virtual void visitNilAST(const ast::NilAST &) {}

        virtual void visitBindingAST(const ast::BindingAST &) {}

        virtual void visitValueBindingAST(const ast::ValueBindingAST &) {}

        virtual void visitLambdaAST(const ast::LambdaAST &) {}

        virtual void visitBuiltinConsAST(const ast::BuiltinConsAST &) {}

        virtual void visitBuiltinCarAST(const ast::BuiltinCarAST &) {}

        virtual void visitBuiltinCdrAST(const ast::BuiltinCdrAST &) {}

        virtual void visitBuiltinAddAST(const ast::BuiltinAddAST &) {}

        virtual void visitBuiltinMultiplyAST(const ast::BuiltinMultiplyAST &) {}

        virtual void visitBuiltinListAST(const ast::BuiltinListAST &) {}

        virtual void visitBuiltinNullAST(const ast::BuiltinNullAST &) {}

        virtual void visitBuiltinLessThanAST(const ast::BuiltinLessThanAST &) {}

        virtual void visitBuiltinOppositeAST(const ast::BuiltinOppositeAST &) {}

        virtual void visitBuiltinReciprocalAST(const ast::BuiltinReciprocalAST &) {}

        virtual void visitLambdaBindingAST(const ast::LambdaBindingAST &) {}

        virtual void visitLambdaApplicationAST(const ast::InvocationAST &) {}

        virtual void visitBuiltinDrawAST(const ast::BuiltinDrawAST &) {}
    };

    class DisplayVisitor : public NodeVisitor {
    public:
        void visitBooleansFalseAST(const ast::BooleansFalseAST &) override;

        void visitBooleansTrueAST(const ast::BooleansTrueAST &) override;

        void visitNumberAST(const ast::NumberAST &) override;

        void visitPairAST(const ast::PairAST &) override;

        void visitNilAST(const ast::NilAST &) override;

        void visitLambdaAST(const ast::LambdaAST &) override;

        std::string to_string() const;

        void clear();

    private:
        std::string prettyPrint;
    };
}


#endif //GI_VISITOR_H
