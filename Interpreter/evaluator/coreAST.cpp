#include <fstream>
#include <sstream>
#include <parser.h>
#include <exception.h>
#include <visitor.h>
#include <context.h>

using namespace parser;
using namespace exception;
using namespace ast;
using namespace visitor;

pExpr LambdaAST::apply(const std::vector<pExpr> &actualArgs, pScope &ss) const {
    // Create new scope
    auto curScope = std::make_shared<Scope>();
    curScope->setLexicalScope(context);
    // Like C with auto declaration: A invokes B while B appears behind A.
    curScope->setDynamicScope(ss);

    for (size_t i = 0; i < actualArgs.size(); i++) {
        if (formalArgs[i] != ".") {
            curScope->addSymbol(formalArgs[i], actualArgs[i]);
        } else {
            curScope->addSymbol(
                formalArgs[i + 1],
                std::make_shared<BuiltinListAST>()->apply(
                    std::vector<std::shared_ptr<ExprAST>>{actualArgs.begin() + i, actualArgs.end()},
                    ss));
            break;
        }
    }
    if (actualArgs.size() == 1 && formalArgs.size() > 1 && formalArgs[1] == ".") {
        curScope->addSymbol(formalArgs[2], std::make_shared<NilAST>());
    }

    // ATTENTION: sub-routes are evaluated here. If something goes wrong, do it in the LambdaAST:eval.
    for (int i = 0; i < expression.size() - 1; i++)
        expression[i]->eval(curScope);
    auto ret = expression.back()->eval(curScope);

    // remove current function name record
    ss->stepOutFunc();
    return ret;
}

std::shared_ptr<ExprAST> LambdaAST::eval(std::shared_ptr<Scope> &ss) const {
    // Create new lambda and operate on it. AST itself should not be changed
    auto lambda = std::make_shared<LambdaAST>(this->formalArgs, this->expression);
    lambda->context->setLexicalScope(ss);

    // ATTENTION: we do not eval sub-routines within lambda->context.

    // Just derive child node. Hence lambda's context won't be modified afterwards.
    auto parent = ss;
    ss = std::make_shared<Scope>();
    ss->setLexicalScope(parent);

    return lambda;
}

std::shared_ptr<ExprAST> LambdaBindingAST::eval(std::shared_ptr<Scope> &ss) const {
    CLOG(DEBUG, "evaluator") << "eval [" << getIdentifier() << "]";
    ss->addSymbol(getIdentifier(), lambda->eval(ss));
    CLOG(DEBUG, "evaluator") << "finish eval [" << getIdentifier() << "]";
    return getPointer();
}

bool IfStatementAST::tailRecursion(const std::shared_ptr<ExprAST> &clause, std::shared_ptr<Scope> &ss) const {
    if (auto callable = std::dynamic_pointer_cast<InvocationAST>(clause)) {
        if (auto id = std::dynamic_pointer_cast<IdentifierAST>(callable->callableObj)) {
            if (id->getId() == ss->currentFunc()) {
                CLOG(DEBUG, "evaluator") << "tail recursion detected " << ss->currentFunc();
                return true;
            }
        }
    }
    return false;
}

std::shared_ptr<ExprAST> IfStatementAST::eval(std::shared_ptr<Scope> &ss) const {
    auto ptr = condition->eval(ss);
    if (auto boolFalsePtr = std::dynamic_pointer_cast<BooleansFalseAST>(ptr)) {
        //TODO: tail recursion optimization
        tailRecursion(falseClause, ss);
        return falseClause->eval(ss);
    } else {
        tailRecursion(trueClause, ss);
        return trueClause->eval(ss);
    }
}

std::shared_ptr<ExprAST> InvocationAST::eval(std::shared_ptr<Scope> &ss) const {
    // Eval the arguments each time: it depends on scope
    std::vector<pExpr> evalRes;
    for (const auto &ptr: actualArgs) evalRes.push_back(ptr->eval(ss));

    std::shared_ptr<ExprAST> ret;

    if (std::dynamic_pointer_cast<LambdaAST>(callableObj)) {
        ss->stepIntoAnonymousFunc();
        ret = callableObj->apply(evalRes, ss);

    } else if (auto id = std::dynamic_pointer_cast<IdentifierAST>(callableObj)) {
        auto lambda = id->eval(ss);
        ss->stepIntoFunc(id->getId());
        ret = lambda->apply(evalRes, ss);
    } else {
        // it may be a function call which returns lambda, so just eval it first
        auto lambda = callableObj->eval(ss);
        ss->stepIntoAnonymousFunc();
        ret = lambda->apply(evalRes, ss);
    }
    return ret;
}

