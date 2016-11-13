#include <AST.h>

using namespace parser;

std::shared_ptr<ExprAST> ExprAST::eval(Scope &) const {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be evaluated.");
};

std::shared_ptr<ExprAST> ExprAST::apply(const std::vector<std::shared_ptr<ExprAST>> &, const Scope &) {
    CLOG(DEBUG, "exception");
    throw std::logic_error("Expression cannot be applied.");
};


std::shared_ptr<ExprAST> NumberAST::eval(Scope &) const {
    return std::make_shared<NumberAST>(getValue());
}


std::shared_ptr<ExprAST> IdentifierAST::eval(Scope &ss) const {
    CLOG(DEBUG, "parser") << "Evaluate identifier: " << getId();
    if (ss.count(getId())) {
        return ss[getId()]->eval(ss);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Unbound identifier.");
    }
}

void ArgumentsAST::bindArguments(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &s) {
    CLOG(DEBUG, "parser") << "Number of formal arguments is " << formalArgs.size();
    for (size_t i = 0; i < actualArgs.size(); i++) {
        CLOG(DEBUG, "parser") << "Set formal argument: " << formalArgs[i];
        s[formalArgs[i]] = actualArgs[i]->eval(s);
    }
}


std::shared_ptr<ExprAST> LambdaAST::apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs,
                                          const Scope &)  {
    bindArguments(actualArgs, context);
    return expression->eval(context);
}


std::shared_ptr<ExprAST> AddOperatorAST::eval(Scope &s) const  {
    double num = 0;
    for (auto element: actualArgs) {
        std::shared_ptr<ExprAST> res = element->eval(s);
        if (auto p = std::dynamic_pointer_cast<NumberAST>(res)) {
            CLOG(DEBUG, "parser") << "Add number: " << p->getValue();
            num += p->getValue();
        } else {
            CLOG(DEBUG, "exception");
            throw std::logic_error("The operands cannot be converted to number");
        }
    }
    return std::make_shared<NumberAST>(num);
}


std::shared_ptr<ExprAST> ValueBindingAST::eval(Scope &ss) const  {
    ss[getIdentifier()] = value->eval(ss);
    return nullptr;
}


std::shared_ptr<ExprAST> LambdaBindingAST::eval(Scope &ss) const  {
    lambda->setContext(ss);
    ss[getIdentifier()] = lambda;
    return nullptr;
}

std::shared_ptr<ExprAST> LambdaApplicationAST::eval(Scope &ss) const  {
    return lambda->apply(actualArgs, ss);
}

std::shared_ptr<ExprAST> FunctionApplicationAST::eval(Scope &ss) const  {
    CLOG(DEBUG, "parser") << "Apply function call. Number of actual arguments: " << actualArgs.size();
    if (ss.count(identifier)) {
        return ss[identifier]->apply(actualArgs, ss);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Unbound function identifier.");
    }
}
