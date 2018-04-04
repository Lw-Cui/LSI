#include <fstream>
#include <sstream>
#include <parser.h>
#include <exception.h>
#include <visitor.h>
#include <AST.h>
#include <builtinAST.h>
#include <context.h>

using namespace parser;
using namespace exception;
using namespace ast;
using namespace visitor;

void BuiltinDrawAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinDrawAST(*this);
}

pExpr BuiltinDrawAST::getPointer() const {
    return std::make_shared<BuiltinDrawAST>(*this);
}


pExpr BuiltinNullAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {

    s->stepOutFunc();
    if (auto p = std::dynamic_pointer_cast<NilAST>(actualArgs.front()))
        return std::make_shared<BooleansTrueAST>();
    else
        return std::make_shared<BooleansFalseAST>();
}

void BuiltinNullAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinNullAST(*this);
}

pExpr BuiltinNullAST::getPointer() const {
    return std::make_shared<BuiltinNullAST>(*this);
}

pExpr BuiltinOppositeAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front())) {
        s->stepOutFunc();
        return std::make_shared<NumberAST>(-p->getValue());
    } else {
        throw NotNumber("The operands cannot be converted to number");
    }
}

void BuiltinOppositeAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinOppositeAST(*this);
}

pExpr BuiltinOppositeAST::getPointer() const {
    return std::make_shared<BuiltinOppositeAST>(*this);
}

pExpr BuiltinListAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    std::shared_ptr<ExprAST> list = std::make_shared<NilAST>();
    for (int i = static_cast<int>(actualArgs.size() - 1); i >= 0; i--)
        list = std::make_shared<PairAST>(actualArgs[i], list);

    s->stepOutFunc();
    return list;
}

void BuiltinListAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinListAST(*this);
}

pExpr BuiltinListAST::getPointer() const {
    return std::make_shared<BuiltinListAST>(*this);
}


pExpr BuiltinLessThanAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    bool res = std::is_sorted(
        std::begin(actualArgs), std::end(actualArgs),
        [&](std::shared_ptr<ExprAST> p1, std::shared_ptr<ExprAST> p2) {
            //[&](decltype(actualArgs)::value_type p1, decltype(actualArgs)::value_type p2) {
            auto np1 = std::dynamic_pointer_cast<NumberAST>(p1);
            auto np2 = std::dynamic_pointer_cast<NumberAST>(p2);
            if (np1 && np2) {
                // Important: if (comp(*next,*first)) return true then is_sorted return false
                return np1->getValue() <= np2->getValue();
            } else {
                throw NotNumber("The operands in less than operator cannot be converted to number");
            }
        });

    s->stepOutFunc();
    if (res) return std::make_shared<BooleansTrueAST>();
    else return std::make_shared<BooleansFalseAST>();
}

void BuiltinLessThanAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinLessThanAST(*this);
}

pExpr BuiltinLessThanAST::getPointer() const {
    return std::make_shared<BuiltinLessThanAST>(*this);
}


pExpr BuiltinCarAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front())) {

        s->stepOutFunc();
        return p->data.first;
    } else {
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCarAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCarAST(*this);
}

pExpr BuiltinCarAST::getPointer() const {
    return std::make_shared<BuiltinCarAST>(*this);
}

pExpr BuiltinCdrAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front())) {

        s->stepOutFunc();
        return p->data.second;
    } else {
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCdrAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCdrAST(*this);
}

pExpr BuiltinCdrAST::getPointer() const {
    return std::make_shared<BuiltinCdrAST>(*this);
}

pExpr BuiltinConsAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (actualArgs.size() == 2) {
        auto p = std::make_shared<PairAST>(actualArgs[0]->eval(s), actualArgs[1]->eval(s));
        s->stepOutFunc();
        return p;
    } else {
        throw NotPair("Builtin cons error.");
    }
}

void BuiltinConsAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinConsAST(*this);
}

pExpr BuiltinConsAST::getPointer() const {
    return std::make_shared<BuiltinConsAST>(*this);
}

pExpr BuiltinAddAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    double num = 0;
    for (auto res: actualArgs) {
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num += p->getValue();
        } else {
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    s->stepOutFunc();
    return std::make_shared<NumberAST>(num);
}

void BuiltinAddAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinAddAST(*this);
}

pExpr BuiltinAddAST::getPointer() const {
    return std::make_shared<BuiltinAddAST>(*this);
}

pExpr BuiltinMultiplyAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    double num = 1;
    for (auto res: actualArgs) {
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num *= p->getValue();
        } else {
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    s->stepOutFunc();
    return std::make_shared<NumberAST>(num);
}

void BuiltinMultiplyAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinMultiplyAST(*this);
}

pExpr BuiltinMultiplyAST::getPointer() const {
    return std::make_shared<BuiltinMultiplyAST>(*this);
}

pExpr
BuiltinReciprocalAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front())) {

        s->stepOutFunc();
        return std::make_shared<NumberAST>(1 / p->getValue());
    } else {
        throw NotNumber("The operands cannot be converted to number");
    }
}

void BuiltinReciprocalAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinReciprocalAST(*this);
}

pExpr BuiltinReciprocalAST::getPointer() const {
    return std::make_shared<BuiltinReciprocalAST>(*this);
}
