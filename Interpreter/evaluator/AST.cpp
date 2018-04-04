#include <fstream>
#include <sstream>
#include <parser.h>
#include <exception.h>
#include <visitor.h>
#include <AST.h>
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

std::vector<pExpr> LoadingFileAST::evalAll(std::shared_ptr<Scope> &s) const {
    std::ifstream fin{filename};
    std::string str{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    lexers::Lexer lex{str};
    auto ptr = std::dynamic_pointer_cast<AllExprAST>(parseAllExpr(lex));
    return ptr->evalAll(s);
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

std::vector<pExpr> AllExprAST::evalAll(std::shared_ptr<Scope> &s) const {
    std::vector<pExpr> ret;
    for (auto ptr : exprVec)
        if (auto load = std::dynamic_pointer_cast<LoadingFileAST>(ptr)) {
            auto vec = load->evalAll(s);
            ret.insert(std::end(ret), std::begin(vec), std::end(vec));
        } else {
            ret.push_back(ptr->eval(s));
        }
    return std::move(ret);
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

std::shared_ptr<ExprAST> LetStatementAST::eval(std::shared_ptr<Scope> &s) const {
    auto tmp = std::make_shared<Scope>();
    tmp->setDynamicScope(s);
    for (auto index = 0; index < identifier.size(); index++) {
        auto id = std::dynamic_pointer_cast<IdentifierAST>(identifier[index])->getId();
        tmp->addSymbol(id, value[index]->eval(s));
    }
    return expr->eval(tmp);
}

