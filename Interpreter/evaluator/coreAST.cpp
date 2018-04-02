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

void LambdaAST::setArgs(const std::vector<pExpr> &actualArgs, std::shared_ptr<Scope> &ss) const {
    for (size_t i = 0; i < actualArgs.size(); i++) {
        if (formalArgs[i] != ".") {
            ss->addSymbol(formalArgs[i], actualArgs[i]);
        } else {
            // Attention: builtinList has to register itself
            ss->stepIntoFunc("list");
            ss->addSymbol(
                formalArgs[i + 1],
                std::make_shared<BuiltinListAST>()->apply(
                    std::vector<std::shared_ptr<ExprAST>>{actualArgs.begin() + i, actualArgs.end()},
                    ss));
            break;
        }
    }
    if (actualArgs.size() == 1 && formalArgs.size() > 1 && formalArgs[1] == ".") {
        ss->addSymbol(formalArgs[2], std::make_shared<NilAST>());
    }
}


pExpr LambdaAST::apply(const std::vector<pExpr> &actualArgs, pScope &ss) const {
    auto args = actualArgs;
    pExpr ret = nullptr;
    auto curScope = std::make_shared<Scope>();
    do {
        // Create new scope
        curScope->clearCurScope();
        curScope->setLexicalScope(context);
        // Like C with auto declaration: A invokes B while B appears behind A.
        curScope->setDynamicScope(ss);
        setArgs(args, curScope);

        for (int i = 0; i < expression.size() - 1; i++)
            // Don't eval sub-routine. It has been evaluated in LambdaAST::eval()
            // otherwise the temp scope will be stored in sub-routine, which consumes billions of bytes.
            if (!std::dynamic_pointer_cast<LambdaBindingAST>(expression[i]))
                expression[i]->eval(curScope);

        ret = expression.back()->eval(curScope);

        if (auto ptr = std::dynamic_pointer_cast<TailRecursionArgs>(ret)) {
            args = ptr->actualArgs;
        } else
            break;

    } while (true);

    // remove current function name record
    ss->stepOutFunc();
    return ret;
}

std::shared_ptr<ExprAST> LambdaAST::eval(std::shared_ptr<Scope> &ss) const {
    // Create new lambda and operate on it. AST itself should not be changed
    auto lambda = std::make_shared<LambdaAST>(this->formalArgs, this->expression);

    // store current context;
    // it has been done before sub-routine evaluation since eval add new node to context
    lambda->context->setLexicalScope(ss);

    /*
     * Here we evaluate the sub-routine rather than lambda body.
     * (define (SUM n acc)
     *      (define (x a) a)
     *      (define (b a) a)
     *      (if (< n 0) acc (SUM (- n 1) (+ acc n))))
     *
     *     (SUM ...)
     *      /   \
     *    (x)  (extern scope)
     *    /
     *  (b)
     */
    for (auto expr: expression)
        if (std::shared_ptr<LambdaBindingAST> ptr = std::dynamic_pointer_cast<LambdaBindingAST>(expr)) {
            // Add sub-routine into original context of this lambda
            ptr->eval(lambda->context);
        }

    // Just derive child node. Hence lambda's context won't be modified afterwards.
    auto parent = ss;
    ss = std::make_shared<Scope>();
    ss->setLexicalScope(parent);

    return lambda;
}

std::shared_ptr<ExprAST> LambdaBindingAST::eval(std::shared_ptr<Scope> &ss) const {
    CLOG(DEBUG, "evaluator") << "eval [" << getIdentifier() << "]";
    ss->addSymbol(getIdentifier(), lambda->eval(ss));
    CLOG(DEBUG, "evaluator") << "eval [" << getIdentifier() << "] done";
    return getPointer();
}

std::vector<pExpr> IfStatementAST::getTailRecursionArgs(
    const std::shared_ptr<ExprAST> &clause, std::shared_ptr<Scope> &ss) const {
    if (auto callable = std::dynamic_pointer_cast<InvocationAST>(clause)) {
        if (auto id = std::dynamic_pointer_cast<IdentifierAST>(callable->callableObj)) {
            if (id->getId() == ss->currentFunc()) {
                CLOG(DEBUG, "evaluator") << "tail recursion detected " << ss->currentFunc();
                std::vector<pExpr> evalRes;
                for (const auto &ptr: callable->actualArgs) evalRes.push_back(ptr->eval(ss));
                CLOG(DEBUG, "evaluator") << "tail recursion arguments done" << ss->currentFunc();
                return std::move(evalRes);
            }
        }
    }
    return std::vector<pExpr>();
}

std::shared_ptr<ExprAST> IfStatementAST::eval(std::shared_ptr<Scope> &ss) const {
    auto ptr = condition->eval(ss);
    if (auto boolFalsePtr = std::dynamic_pointer_cast<BooleansFalseAST>(ptr)) {
        return eval(falseClause, ss);
    } else {
        return eval(trueClause, ss);
    }
}

std::shared_ptr<ExprAST> IfStatementAST::eval(
    const std::shared_ptr<ExprAST> &branch, std::shared_ptr<Scope> &ss) const {
    auto args = getTailRecursionArgs(branch, ss);
    if (!args.empty()) {
        return std::make_shared<TailRecursionArgs>(args);
    } else {
        return branch->eval(ss);
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

