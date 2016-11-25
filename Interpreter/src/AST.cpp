#include <fstream>
#include <parser.h>
#include <lexers.h>
#include <AST.h>

using namespace parser;
using namespace ast;

std::shared_ptr<ExprAST> ExprAST::eval(Scope &) const {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be evaluated.");
};

std::shared_ptr<ExprAST> ExprAST::apply(const std::vector<std::shared_ptr<ExprAST>> &, Scope &) {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be applied.");
}

std::shared_ptr<ExprAST> LoadingFileAST::eval(Scope &s) const {
    std::ifstream fin{filename};
    std::string str{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    lexers::Lexer lex{str};
    parseAllExpr(lex)->eval(s);
    return nullptr;
}

std::shared_ptr<ExprAST> IfStatementAST::eval(Scope &ss) const {
    auto ptr = condition->eval(ss);
    if (auto boolFalsePtr = std::dynamic_pointer_cast<BooleansFalseAST>(ptr)) {
        return falseClause->eval(ss);
    }
    return trueClause->eval(ss);
}

std::shared_ptr<ExprAST> BuiltinLessThanAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    bool res = std::is_sorted(
            std::begin(actualArgs), std::end(actualArgs),
            [&](std::shared_ptr<ExprAST> p1, std::shared_ptr<ExprAST> p2) {
                //[&](decltype(actualArgs)::value_type p1, decltype(actualArgs)::value_type p2) {
                auto np1 = std::dynamic_pointer_cast<NumberAST>(p1->eval(s));
                auto np2 = std::dynamic_pointer_cast<NumberAST>(p2->eval(s));
                if (np1 && np2) {
                    // Important: if (comp(*next,*first)) return true then is_sorted return false
                    return np1->getValue() <= np2->getValue();
                } else {
                    CLOG(DEBUG, "exception");
                    throw std::logic_error("The operands in less than operator cannot be converted to number");
                }
            });
    if (res) return std::make_shared<BooleansTrueAST>();
    else return std::make_shared<BooleansFalseAST>();
}

std::shared_ptr<ExprAST> NumberAST::eval(Scope &) const {
    return std::make_shared<NumberAST>(getValue());
}

std::shared_ptr<ExprAST> IdentifierAST::eval(Scope &ss) const {
    if (ss.count(getId())) {
        return ss[getId()];
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Unbound identifier: " + getId());
    }
}

std::shared_ptr<ExprAST> LambdaAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &ss) {
    // Backup scope of lambda. If not, recursive calls will destroy scope by binding arguments.
    Scope tmp = context;
    // If external scope contains some identifier needed by this lambda, update it.
    for (auto iter: ss)
        if (!tmp.count(iter.first))
            tmp[iter.first] = iter.second;
    for (size_t i = 0; i < actualArgs.size(); i++) {
        if (formalArgs[i] != ".") {
            // Evaluate value from current scope and set them into scope of lambda
            tmp[formalArgs[i]] = actualArgs[i]->eval(ss);
        } else {
            tmp[formalArgs[i + 1]] =
                    std::make_shared<BuiltinListAST>()->apply(
                            std::vector<std::shared_ptr<ExprAST>>{actualArgs.begin() + i, actualArgs.end()},
                            ss);
            break;
        }
    }
    if (actualArgs.size() == 1 && formalArgs.size() > 1 && formalArgs[1] == ".")
        tmp[formalArgs[2]] = std::make_shared<NilAST>()->eval(ss);
    return expression->eval(tmp);
}

std::shared_ptr<ExprAST> LambdaAST::eval(Scope &ss) const {
    // Set closure.
    context = ss;
    for (auto expr : nestedFunc) expr->eval(context);
    return std::make_shared<LambdaAST>(*this);
}

std::shared_ptr<ExprAST> BuiltinOppositeAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front()->eval(s))) {
        return std::make_shared<NumberAST>(-p->getValue());
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("The operands cannot be converted to number");
    }
}

std::shared_ptr<ExprAST> BuiltinListAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    std::shared_ptr<ExprAST> list = std::make_shared<NilAST>();
    for (int i = static_cast<int>(actualArgs.size() - 1); i >= 0; i--)
        list = std::make_shared<PairAST>(actualArgs[i]->eval(s), list);
    return list;
}

std::shared_ptr<ExprAST> ValueBindingAST::eval(Scope &ss) const {
    ss[getIdentifier()] = value->eval(ss);
    return nullptr;
}

std::shared_ptr<ExprAST> LambdaBindingAST::eval(Scope &ss) const {
    // Set closure.
    lambda->eval(ss);
    // Set its identifier into its context to support recursion. Cannot assign lambda->eval(ss) to others!
    ss[getIdentifier()] = lambda->upgradeScope()[getIdentifier()] = lambda;
    return nullptr;
}

std::shared_ptr<ExprAST> LambdaApplicationAST::eval(Scope &ss) const {
    if (std::dynamic_pointer_cast<LambdaAST>(lambdaOrIdentifier)) {
        return lambdaOrIdentifier->apply(actualArgs, ss);
    } else {
        return lambdaOrIdentifier->eval(ss)->apply(actualArgs, ss);
    }
}

std::shared_ptr<ExprAST> BooleansTrueAST::eval(Scope &) const {
    return std::make_shared<BooleansTrueAST>();
}

std::shared_ptr<ExprAST> BooleansFalseAST::eval(Scope &) const {
    return std::make_shared<BooleansFalseAST>();
}

std::shared_ptr<ExprAST> AllExprAST::eval(Scope &s) const {
    std::shared_ptr<ExprAST> res;
    for (auto ptr : exprVec)
        res = ptr->eval(s);
    return res;
}

std::shared_ptr<ExprAST> PairAST::eval(Scope &s) const {
    return std::make_shared<PairAST>(data.first->eval(s), data.second->eval(s));
}

std::shared_ptr<ExprAST> BuiltinNullAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (auto p = std::dynamic_pointer_cast<NilAST>(actualArgs.front()->eval(s)))
        return std::make_shared<BooleansTrueAST>();
    else
        return std::make_shared<BooleansFalseAST>();
}

std::shared_ptr<ExprAST> NilAST::eval(Scope &s) const {
    return std::make_shared<NilAST>(*this);
}

std::shared_ptr<ExprAST> BuiltinCarAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front()->eval(s))) {
        return p->data.first;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Cannot convert to pair");
    }
}

std::shared_ptr<ExprAST> BuiltinCdrAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (auto p = std::dynamic_pointer_cast<PairAST>(actualArgs.front()->eval(s))) {
        return p->data.second;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Cannot convert to pair");
    }
}

std::shared_ptr<ExprAST> BuiltinConsAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (actualArgs.size() == 2) {
        return std::make_shared<PairAST>(actualArgs[0]->eval(s), actualArgs[1]->eval(s));
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Builtin cons error.");
    }
}

std::shared_ptr<ExprAST> BuiltinAddAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    double num = 0;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num += p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw std::logic_error("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

std::shared_ptr<ExprAST> BuiltinMultiplyAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    double num = 1;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            num *= p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw std::logic_error("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}

std::shared_ptr<ExprAST>
BuiltinReciprocalAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    if (auto p = std::dynamic_pointer_cast<NumberAST>(actualArgs.front()->eval(s))) {
        return std::make_shared<NumberAST>(1 / p->getValue());
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("The operands cannot be converted to number");
    }
}

CondStatementAST::CondStatementAST(const std::vector<std::shared_ptr<ExprAST>> &condition,
                                   const std::vector<std::shared_ptr<ExprAST>> &result)
        : ifStatement{std::make_shared<BooleansFalseAST>()} {
    for (int index = static_cast<int>(condition.size() - 1); index >= 0; index--)
        ifStatement = std::make_shared<IfStatementAST>(condition[index], result[index], ifStatement);
}

std::shared_ptr<ExprAST> CondStatementAST::eval(Scope &s) const {
    return ifStatement->eval(s);
}

std::shared_ptr<ExprAST> LetStatementAST::eval(Scope &s) const {
    Scope tmp = s;
    for (auto index = 0; index < identifier.size(); index++) {
        auto id = std::dynamic_pointer_cast<IdentifierAST>(identifier[index])->getId();
        tmp[id] = value[index]->eval(tmp);
    }
    return expr->eval(tmp);
}
