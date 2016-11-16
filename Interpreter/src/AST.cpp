#include <fstream>
#include <parser.h>
#include <lexers.h>
#include <AST.h>

using namespace parser;

std::shared_ptr<ExprAST> ExprAST::eval(Scope &) const {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be evaluated.");
};

std::shared_ptr<ExprAST> ExprAST::apply(const std::vector<std::shared_ptr<ExprAST>> &, const Scope &) {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be applied.");
}

std::shared_ptr<ExprAST> ExprAST::toBool(Scope &s) const {
    return std::make_shared<BooleansAST>(true)->eval(s);
};

std::shared_ptr<ExprAST> LoadingFileAST::eval(Scope &s) const {
    std::ifstream fin{filename};
    std::string str{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    lexers::Lexer lex{str};
    while (lex.getTokType() != lexers::Lexer::TokEOF)
        parseExpr(lex)->eval(s);
    return nullptr;
}

std::shared_ptr<ExprAST> IfStatementAST::eval(Scope &ss) const {
    if (condition->eval(ss)->toBool(ss)) {
        CLOG(DEBUG, "AST") << "Evaluate true clause.";
        return trueClause->eval(ss);
    } else {
        CLOG(DEBUG, "AST") << "Evaluate false clause.";
        return falseClause->eval(ss);
    }
}

std::shared_ptr<ExprAST> LessThanOperatorAST::eval(Scope &s) const {
    bool res = std::is_sorted(
            std::begin(actualArgs), std::end(actualArgs),
            [&](std::shared_ptr<ExprAST> p1, std::shared_ptr<ExprAST> p2) {
                //[&](decltype(actualArgs)::value_type p1, decltype(actualArgs)::value_type p2) {
                auto np1 = std::dynamic_pointer_cast<NumberAST>(p1->eval(s));
                auto np2 = std::dynamic_pointer_cast<NumberAST>(p2->eval(s));
                if (np1 && np2) {
                    return np1->getValue() < np2->getValue();
                } else {
                    CLOG(DEBUG, "exception");
                    throw std::logic_error(
                            "The operands in less than operator cannot be converted to number");
                }
            });
    if (res) return std::make_shared<NumberAST>(1);
    else return std::make_shared<NumberAST>(0);
}

std::shared_ptr<ExprAST> NumberAST::eval(Scope &) const {
    return std::make_shared<NumberAST>(getValue());
}

std::shared_ptr<ExprAST> NumberAST::toBool(Scope &s) const {
    return std::make_shared<BooleansAST>(getValue() != 0)->eval(s);
}

std::shared_ptr<ExprAST> IdentifierAST::eval(Scope &ss) const {
    CLOG(DEBUG, "AST") << "Evaluate identifier: " << getId();
    if (ss.count(getId())) {
        return ss[getId()];
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Unbound identifier.");
    }
}

void ArgumentsAST::bindArguments(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    CLOG(DEBUG, "AST") << "Number of formal arguments is " << formalArgs.size();
    for (size_t i = 0; i < actualArgs.size(); i++) {
        CLOG(DEBUG, "AST") << "Set formal argument: " << formalArgs[i];
        s[formalArgs[i]] = actualArgs[i]->eval(s);
    }
}

std::shared_ptr<ExprAST> LambdaAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs,
                                          const Scope &) {
    bindArguments(actualArgs, context);
    return expression->eval(context);
}


std::shared_ptr<ExprAST> AddOperatorAST::eval(Scope &s) const {
    double num = 0;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            CLOG(DEBUG, "AST") << "Add number: " << p->getValue();
            num += p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw std::logic_error("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}


std::shared_ptr<ExprAST> ValueBindingAST::eval(Scope &ss) const {
    ss[getIdentifier()] = value->eval(ss);
    return nullptr;
}


std::shared_ptr<ExprAST> LambdaBindingAST::eval(Scope &ss) const {
    lambda->setContext(ss);
    ss[getIdentifier()] = lambda;
    return nullptr;
}

std::shared_ptr<ExprAST> LambdaApplicationAST::eval(Scope &ss) const {
    if (std::dynamic_pointer_cast<LambdaAST>(lambda)) {
        return lambda->apply(actualArgs, ss);
    } else {
        return lambda->eval(ss)->apply(actualArgs, ss);
    }
}

std::shared_ptr<ExprAST> FunctionApplicationAST::eval(Scope &ss) const {
    CLOG(DEBUG, "AST") << "Apply function call. Number of actual arguments: " << actualArgs.size();
    if (ss.count(identifier)) {
        return ss[identifier]->apply(actualArgs, ss);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Unbound function identifier.");
    }
}

std::shared_ptr<ExprAST> BooleansAST::eval(Scope &) const {
    if (booleans)
        return std::make_shared<BooleansAST>(*this);
    else
        return nullptr;
}
