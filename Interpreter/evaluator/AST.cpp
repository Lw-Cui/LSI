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

void LambdaBindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaBindingAST(*this);
}

pExpr LambdaBindingAST::getPointer() const {
    return std::make_shared<LambdaBindingAST>(*this);
}

std::shared_ptr<ExprAST> ValueBindingAST::eval(std::shared_ptr<Scope> &ss) const {
    ss->addSymbol(getIdentifier(), value->eval(ss));
    return getPointer();
}

void ValueBindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBindingAST(*this);
}

pExpr ValueBindingAST::getPointer() const {
    return std::make_shared<ValueBindingAST>(*this);
}

ValueBindingAST::ValueBindingAST(const std::string &id, const std::shared_ptr<ExprAST> &v)
    : BindingAST(id), value{v} {}

std::shared_ptr<ExprAST> ExprAST::eval(std::shared_ptr<Scope> &) const {
    return getPointer();
};

pExpr ExprAST::getPointer() const {
    return std::make_shared<ExprAST>(*this);
}

pExpr ExprAST::apply(const std::vector<pExpr> &, pScope &) const {
    throw RuntimeError("Expression cannot be applied.");
}

void ExprAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitExprAST(*this);
}

std::shared_ptr<ExprAST> LoadingFileAST::eval(std::shared_ptr<Scope> &s) const {
    std::ifstream fin{filename};
    std::string str{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    lexers::Lexer lex{str};
    auto ptr = parseAllExpr(lex);
    return ptr->eval(s);
}

void LoadingFileAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLoadingFileAST(*this);
}

pExpr LoadingFileAST::getPointer() const {
    return std::make_shared<LoadingFileAST>(*this);
}

void IfStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIfStatementAST(*this);
}

pExpr IfStatementAST::getPointer() const {
    return std::make_shared<IfStatementAST>(*this);
}

IfStatementAST::IfStatementAST(const std::shared_ptr<ExprAST> &c, const std::shared_ptr<ExprAST> &t,
                               const std::shared_ptr<ExprAST> &f) :
    condition{c}, trueClause{t}, falseClause{f} {}


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
    if (res) return std::make_shared<BooleansTrueAST>();
    else return std::make_shared<BooleansFalseAST>();
}

void BuiltinLessThanAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinLessThanAST(*this);
}

pExpr BuiltinLessThanAST::getPointer() const {
    return std::make_shared<BuiltinLessThanAST>(*this);
}


void NumberAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitNumberAST(*this);
}

pExpr NumberAST::getPointer() const {
    return std::make_shared<NumberAST>(*this);
}

std::shared_ptr<ExprAST> IdentifierAST::eval(std::shared_ptr<Scope> &ss) const {
    if (ss->count(getId())) {
        return ss->findSymbol(getId());
    } else {
        throw UnboundIdentifier("Unbound identifier: " + getId());
    }
}

void IdentifierAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIdentifierAST(*this);
}

pExpr IdentifierAST::getPointer() const {
    return std::make_shared<IdentifierAST>(*this);
}


pExpr BuiltinOppositeAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front())) {
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
    return list;
}

void BuiltinListAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinListAST(*this);
}

pExpr BuiltinListAST::getPointer() const {
    return std::make_shared<BuiltinListAST>(*this);
}

void InvocationAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaApplicationAST(*this);
}

pExpr InvocationAST::getPointer() const {
    return std::make_shared<InvocationAST>(*this);
}

InvocationAST::InvocationAST(const std::shared_ptr<ExprAST> &lam, const std::vector<std::shared_ptr<ExprAST>> &args)
    : callableObj{lam}, actualArgs{args} {
}

void BooleansTrueAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBooleansTrueAST(*this);
}

pExpr BooleansTrueAST::getPointer() const {
    return std::make_shared<BooleansTrueAST>(*this);
}

void BooleansFalseAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBooleansFalseAST(*this);
}

pExpr BooleansFalseAST::getPointer() const {
    return std::make_shared<BooleansFalseAST>(*this);
}

std::shared_ptr<ExprAST> AllExprAST::eval(std::shared_ptr<Scope> &s) const {
    pExpr ret;
    for (auto ptr : exprVec)
        ret = ptr->eval(s);
    return ret;
}

void AllExprAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitAllExprAST(*this);
}

pExpr AllExprAST::getPointer() const {
    return std::make_shared<AllExprAST>(*this);
}

std::shared_ptr<ExprAST> PairAST::eval(std::shared_ptr<Scope> &s) const {
    // Attention: since the evaluation may depends on context, never store those result.
    return std::make_shared<PairAST>(data.first->eval(s), data.second->eval(s));
}

void PairAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitPairAST(*this);
}

pExpr PairAST::getPointer() const {
    return std::make_shared<PairAST>(*this);
}

pExpr BuiltinNullAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
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


void LambdaAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLambdaAST(*this);
}

pExpr LambdaAST::getPointer() const {
    return std::make_shared<LambdaAST>(*this);
}

LambdaAST::LambdaAST(std::vector<std::string> v, std::vector<std::shared_ptr<ExprAST>> expr)
    : formalArgs{std::move(v)}, expression{std::move(expr)}, context{new Scope} {}


void NilAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitNilAST(*this);
}

pExpr NilAST::getPointer() const {
    return std::make_shared<NilAST>(*this);
}

pExpr BuiltinCarAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) const {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front())) {
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
        return std::make_shared<PairAST>(actualArgs[0]->eval(s), actualArgs[1]->eval(s));
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

CondStatementAST::CondStatementAST(const std::vector<std::shared_ptr<ExprAST>> &condition,
                                   const std::vector<std::shared_ptr<ExprAST>> &result)
    : ifStatement{std::make_shared<BooleansFalseAST>()} {
    for (int index = static_cast<int>(condition.size() - 1); index >= 0; index--)
        ifStatement = std::make_shared<IfStatementAST>(condition[index], result[index], ifStatement);
}

std::shared_ptr<ExprAST> CondStatementAST::eval(std::shared_ptr<Scope> &s) const {
    return ifStatement->eval(s);
}

void CondStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitCondStatementAST(*this);
}

pExpr CondStatementAST::getPointer() const {
    return std::make_shared<CondStatementAST>(*this);
}

void LetStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitLetStatementAST(*this);
}

pExpr LetStatementAST::getPointer() const {
    return std::make_shared<LetStatementAST>(*this);
}

void BindingAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBindingAST(*this);
}

pExpr BindingAST::getPointer() const {
    return std::make_shared<BindingAST>(*this);
}

void BuiltinDrawAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinDrawAST(*this);
}

pExpr BuiltinDrawAST::getPointer() const {
    return std::make_shared<BuiltinDrawAST>(*this);
}

std::shared_ptr<ExprAST> LetStatementAST::eval(std::shared_ptr<Scope> &s) const {
    auto tmp = std::make_shared<Scope>();
    tmp->setDynamicScope(s);
    for (auto index = 0; index < identifier.size(); index++) {
        auto id = std::dynamic_pointer_cast<IdentifierAST>(identifier[index])->getId();
        tmp->addSymbol(id, value[index]->eval(s));
    }
    return expr->eval(tmp);
}

