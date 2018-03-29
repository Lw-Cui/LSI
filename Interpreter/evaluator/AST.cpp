#include <fstream>
#include <sstream>
#include <parser.h>
#include <exception.h>
#include <visitor.h>

using namespace parser;
using namespace exception;
using namespace ast;
using namespace visitor;

void LambdaBindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaBindingAST(*this);
}

std::shared_ptr<ExprAST> ValueBindingAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    ss->addName(getIdentifier(), value->eval(ss, value));
    return nullptr;
}

void ValueBindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBindingAST(*this);
}

std::shared_ptr<ExprAST> ExprAST::eval(std::shared_ptr<Scope> &, const pExpr &ret) const {
    return ret;
};

pExpr ExprAST::apply(const std::vector<pExpr>, pScope &) {
    throw RuntimeError("Expression cannot be applied.");
}

void ExprAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitExprAST(*this);
}

std::shared_ptr<ExprAST> LoadingFileAST::eval(std::shared_ptr<Scope> &s, const pExpr &) const {
    std::ifstream fin{filename};
    std::string str{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    lexers::Lexer lex{str};
    auto ptr = parseAllExpr(lex);
    return ptr->eval(s, ptr);
}

void LoadingFileAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLoadingFileAST(*this);
}

void IfStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIfStatementAST(*this);
}

pExpr BuiltinLessThanAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
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
    if (res) return std::make_shared<BooleansTrueAST>();
    else return std::make_shared<BooleansFalseAST>();
}

void BuiltinLessThanAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinLessThanAST(*this);
}


void NumberAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitNumberAST(*this);
}

std::shared_ptr<ExprAST> IdentifierAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    if (ss->count(getId())) {
        return ss->searchName(getId());
    } else {
        throw UnboundIdentifier("Unbound identifier: " + getId());
    }
}

void IdentifierAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIdentifierAST(*this);
}


pExpr BuiltinOppositeAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front())) {
        return std::make_shared<NumberAST>(-p->getValue());
    } else {
        throw NotNumber("The operands cannot be converted to number");
    }
}

void BuiltinOppositeAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinOppositeAST(*this);
}

pExpr BuiltinListAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    std::shared_ptr<ExprAST> list = std::make_shared<NilAST>();
    for (int i = static_cast<int>(actualArgs.size() - 1); i >= 0; i--)
        list = std::make_shared<PairAST>(actualArgs[i], list);
    return list;
}

void BuiltinListAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinListAST(*this);
}

std::shared_ptr<ExprAST> FunctionArgumentAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    return nullptr;
}

const std::vector<std::shared_ptr<ExprAST>> &FunctionArgumentAST::getAcualArgs() const {
    return actualArgs;
}

void InvocationAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaApplicationAST(*this);
}

void BooleansTrueAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBooleansTrueAST(*this);
}

void BooleansFalseAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBooleansFalseAST(*this);
}

std::shared_ptr<ExprAST> AllExprAST::eval(std::shared_ptr<Scope> &s, const pExpr &) const {
    pExpr ret;
    for (auto ptr : exprVec)
        ret = ptr->eval(s, ptr);
    return ret;
}

void AllExprAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitAllExprAST(*this);
}

std::shared_ptr<ExprAST> PairAST::eval(std::shared_ptr<Scope> &s, const pExpr &ret) const {
    data.first = data.first->eval(s, data.first);
    data.second = data.second->eval(s, data.second);
    return ret;
}

void PairAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitPairAST(*this);
}

pExpr BuiltinNullAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NilAST>(actualArgs.front()))
        return std::make_shared<BooleansTrueAST>();
    else
        return std::make_shared<BooleansFalseAST>();
}

void BuiltinNullAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinNullAST(*this);
}


void LambdaAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaAST(*this);
}

void NilAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitNilAST(*this);
}

pExpr BuiltinCarAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front())) {
        return p->data.first;
    } else {
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCarAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCarAST(*this);
}

pExpr BuiltinCdrAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front())) {
        return p->data.second;
    } else {
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCdrAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCdrAST(*this);
}

pExpr BuiltinConsAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (actualArgs.size() == 2) {
        // actualArgs[0]->eval(s, actualArgs[0]) maybe return a new object, so return value matters.
        return std::make_shared<PairAST>(actualArgs[0], actualArgs[1]);
    } else {
        throw NotPair("Builtin cons error.");
    }
}

void BuiltinConsAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinConsAST(*this);
}

pExpr BuiltinAddAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    double num = 0;
    for (auto res: actualArgs) {
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num += p->getValue();
        } else {
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

void BuiltinAddAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinAddAST(*this);
}

pExpr BuiltinMultiplyAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    double num = 1;
    for (auto res: actualArgs) {
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num *= p->getValue();
        } else {
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

void BuiltinMultiplyAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinMultiplyAST(*this);
}

pExpr
BuiltinReciprocalAST::apply(const std::vector<pExpr> actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front())) {
        return std::make_shared<NumberAST>(1 / p->getValue());
    } else {
        throw NotNumber("The operands cannot be converted to number");
    }
}

void BuiltinReciprocalAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinReciprocalAST(*this);
}

CondStatementAST::CondStatementAST(const std::vector<std::shared_ptr<ExprAST>> &condition,
                                   const std::vector<std::shared_ptr<ExprAST>> &result)
    : ifStatement{std::make_shared<BooleansFalseAST>()} {
    for (int index = static_cast<int>(condition.size() - 1); index >= 0; index--)
        ifStatement = std::make_shared<IfStatementAST>(condition[index], result[index], ifStatement);
}

std::shared_ptr<ExprAST> CondStatementAST::eval(std::shared_ptr<Scope> &s, const pExpr &) const {
    return ifStatement->eval(s, ifStatement);
}

void CondStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitCondStatementAST(*this);
}

void LetStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLetStatementAST(*this);
}

void BindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBindingAST(*this);
}

void BuiltinDrawAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinDrawAST(*this);
}
