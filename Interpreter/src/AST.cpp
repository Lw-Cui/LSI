#include <fstream>
#include <sstream>
#include <parser.h>
#include <exception.h>
#include <visitor.h>

using namespace parser;
using namespace exception;
using namespace ast;
using namespace visitor;

pExpr LambdaAST::apply(const std::vector<pExpr> &actualArgs, pScope &ss) {
    // Backup scope of lambda. If not, recursive calls will destroy scope by binding arguments.
    auto tmp = std::make_shared<Scope>();
    tmp->setSearchDomain(context);
    // If external scope contains some identifier needed by this lambda, update it.
    // For example: func A calls func B while func B appears behind func A. Like C with auto declaration.
    tmp->setSearchDomain(ss);

    for (size_t i = 0; i < actualArgs.size(); i++) {
        if (formalArgs[i] != ".") {
            // Evaluate value from current scope and set them into scope of lambda
            tmp->addName(formalArgs[i], actualArgs[i]->eval(ss, actualArgs[i]));
        } else {
            tmp->addName(formalArgs[i + 1],
                         std::make_shared<BuiltinListAST>()->apply(
                                 std::vector<std::shared_ptr<ExprAST>>{actualArgs.begin() + i, actualArgs.end()},
                                 ss));
            break;
        }
    }
    if (actualArgs.size() == 1 && formalArgs.size() > 1 && formalArgs[1] == ".") {
        tmp->addName(formalArgs[2], std::make_shared<NilAST>());
    }

    for (int i = 0; i < expression.size() - 1; i++)
        // Don't eval sub-routine. We've done it (it could be only evaluated once) in LambdaAST::eval
        if (!std::dynamic_pointer_cast<LambdaBindingAST>(expression[i]))
            expression[i]->eval(tmp, expression[i]);
    return expression.back()->eval(tmp, expression.back());
}

std::shared_ptr<ExprAST> LambdaAST::eval(std::shared_ptr<Scope> &ss, const pExpr &ret) const {
    // Here we evaluate the sub-routine.
    // Please note that func body will not be eval here, no binding at all
    // because lambda will be evaluated more than once when regraded as argument, so store it stauts.
    if (!isSubRoutineEvaluated)
        for (auto expr: expression)
            if (std::shared_ptr<LambdaBindingAST> ptr = std::dynamic_pointer_cast<LambdaBindingAST>(expr))
                // Add sub-routine into original context of this lambda
                expr->eval(context, expr);
    isSubRoutineEvaluated = true;

    // Set closure. We operate in a copy of this LambdaAST and cannot change origin one.
    // Reason has been mentioned above.
    auto lambda = std::make_shared<LambdaAST>(*this);
    lambda->context->setSearchDomain(ss);
    ss->openNewScope(ss);

    return lambda;
}

std::shared_ptr<ExprAST> LambdaBindingAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    // Set closure.
    ss->addName(getIdentifier(), lambda->eval(ss, lambda));
    return nullptr;
}

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

pExpr ExprAST::apply(const std::vector<pExpr> &, pScope &) {
    CLOG(DEBUG, "exception");
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

std::shared_ptr<ExprAST> IfStatementAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    auto ptr = condition->eval(ss, condition);
    if (auto boolFalsePtr = std::dynamic_pointer_cast<BooleansFalseAST>(ptr)) {
        return falseClause->eval(ss, falseClause);
    }
    return trueClause->eval(ss, trueClause);
}

void IfStatementAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIfStatementAST(*this);
}

pExpr BuiltinLessThanAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    bool res = std::is_sorted(
            std::begin(actualArgs), std::end(actualArgs),
            [&](std::shared_ptr<ExprAST> p1, std::shared_ptr<ExprAST> p2) {
                //[&](decltype(actualArgs)::value_type p1, decltype(actualArgs)::value_type p2) {
                auto np1 = std::dynamic_pointer_cast<NumberAST>(p1->eval(s, p1));
                auto np2 = std::dynamic_pointer_cast<NumberAST>(p2->eval(s, p2));
                if (np1 && np2) {
                    // Important: if (comp(*next,*first)) return true then is_sorted return false
                    return np1->getValue() <= np2->getValue();
                } else {
                    CLOG(DEBUG, "exception");
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
        std::cout << getId() << std::endl;
        throw UnboundIdentifier("Unbound identifier: " + getId());
    }
}

void IdentifierAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitIdentifierAST(*this);
}


pExpr BuiltinOppositeAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front()->eval(s, actualArgs.front()))) {
        return std::make_shared<NumberAST>(-p->getValue());
    } else {
        CLOG(DEBUG, "exception");
        throw NotNumber("The operands cannot be converted to number");
    }
}

void BuiltinOppositeAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinOppositeAST(*this);
}

pExpr BuiltinListAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    std::shared_ptr<ExprAST> list = std::make_shared<NilAST>();
    for (int i = static_cast<int>(actualArgs.size() - 1); i >= 0; i--)
        list = std::make_shared<PairAST>(actualArgs[i]->eval(s, actualArgs[i]), list);
    return list;
}

void BuiltinListAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinListAST(*this);
}

std::shared_ptr<ExprAST> LambdaApplicationAST::eval(std::shared_ptr<Scope> &ss, const pExpr &) const {
    if (std::dynamic_pointer_cast<LambdaAST>(lambdaOrIdentifier)) {
        return lambdaOrIdentifier->apply(actualArgs, ss);
    } else {
        auto lambda = lambdaOrIdentifier->eval(ss, lambdaOrIdentifier);
        return lambda->apply(actualArgs, ss);
    }
}

void LambdaApplicationAST::accept(visitor::NodeVisitor &visitor) const {
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

pExpr BuiltinNullAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NilAST>(actualArgs.front()->eval(s, actualArgs.front())))
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

pExpr BuiltinCarAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front()->eval(s, actualArgs.front()))) {
        return p->data.first;
    } else {
        CLOG(DEBUG, "exception");
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCarAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCarAST(*this);
}

pExpr BuiltinCdrAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front()->eval(s, actualArgs.front()))) {
        return p->data.second;
    } else {
        CLOG(DEBUG, "exception");
        throw NotPair("Cannot convert to pair");
    }
}

void BuiltinCdrAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinCdrAST(*this);
}

pExpr BuiltinConsAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (actualArgs.size() == 2) {
        // actualArgs[0]->eval(s, actualArgs[0]) maybe return a new object, so return value matters.
        return std::make_shared<PairAST>(actualArgs[0]->eval(s, actualArgs[0]), actualArgs[1]->eval(s, actualArgs[1]));
    } else {
        CLOG(DEBUG, "exception");
        throw NotPair("Builtin cons error.");
    }
}

void BuiltinConsAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinConsAST(*this);
}

pExpr BuiltinAddAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    double num = 0;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s, element);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num += p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

void BuiltinAddAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinAddAST(*this);
}

pExpr BuiltinMultiplyAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    double num = 1;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s, element);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num *= p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw NotNumber("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

void BuiltinMultiplyAST::accept(visitor::NodeVisitor &visitor) const {
    visitor.visitBuiltinMultiplyAST(*this);
}

pExpr
BuiltinReciprocalAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front()->eval(s, actualArgs.front()))) {
        return std::make_shared<NumberAST>(1 / p->getValue());
    } else {
        CLOG(DEBUG, "exception");
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

std::shared_ptr<ExprAST> LetStatementAST::eval(std::shared_ptr<Scope> &s, const pExpr &) const {
    auto tmp = std::make_shared<Scope>();
    tmp->setSearchDomain(s);
    for (auto index = 0; index < identifier.size(); index++) {
        auto id = std::dynamic_pointer_cast<IdentifierAST>(identifier[index])->getId();
        tmp->addName(id, value[index]->eval(s, value[index]));
    }
    return expr->eval(tmp, expr);
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
