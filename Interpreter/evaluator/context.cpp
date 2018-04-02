#include <memory>
#include <unordered_map>
#include <AST.h>
#include <builtinAST.h>
#include <parser.h>
#include <context.h>
#include <stack>
#include <exception.h>

using namespace std;
using namespace ast;

namespace context {

    bool Scope::count(const std::string &str) const {
        return findSymbol(str) != nullptr;
    }


    void Scope::setDynamicScope(const std::shared_ptr<Scope> &scope) {
        dynamicScope = scope;
    }

    void Scope::setLexicalScope(const std::shared_ptr<Scope> &scope) {
        lexicalScope = scope;
    }

    void Scope::clearCurScope() {
        symtab.clear();
    }

    void Scope::addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) {
        symtab[name] = expr;
    }

    pExpr Scope::findSymbol(const std::string &id) const {
        if (builtinFunc.count(id)) return builtinFunc.find(id)->second;
        if (symtab.count(id)) return symtab.find(id)->second;

        pExpr ret = nullptr;
        if (lexicalScope && ((ret = lexicalScope->findSymbol(id)) != nullptr))
            return ret;
        if (dynamicScope && ((ret = dynamicScope->findSymbol(id)) != nullptr))
            return ret;
        return ret;
    }

    void Scope::addSymbol(const std::string &id, pExpr ptr) {
        if (builtinFunc.count(id)) throw std::runtime_error("Cannot overload Builtin func");
        symtab[id] = ptr;
    }

    std::stack<std::string> Scope::callTrace;

    int Scope::anonymousId;


    void Scope::stepIntoFunc(const std::string &name) {
        if (builtinFunc.count(name)) return;
        CLOG(DEBUG, "context") << "call [" << name << "]";
        callTrace.push(name);
    }

    void Scope::stepOutFunc() {
        CLOG(DEBUG, "context") << "finish call [" << callTrace.top() << "]";
        callTrace.pop();
    }

    void Scope::stepIntoAnonymousFunc() {
        stepIntoFunc("(anonymous #" + to_string(anonymousId++) + ")");
    }

    std::string Scope::currentFunc() const {
        return callTrace.top();
    }

    Scope::Scope() {}

    const std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> Scope::builtinFunc = {
        {"cons", make_shared<BuiltinConsAST>()},
        {"car", make_shared<BuiltinCarAST>()},
        {"cdr", make_shared<BuiltinCdrAST>()},
        {"+", make_shared<BuiltinAddAST>()},
        {"*", make_shared<BuiltinMultiplyAST>()},
        {"null?", make_shared<BuiltinNullAST>()},
        {"<", make_shared<BuiltinLessThanAST>()},
        {"#opposite", make_shared<BuiltinOppositeAST>()},
        {"#reciprocal", make_shared<BuiltinReciprocalAST>()},
        {"list", make_shared<BuiltinListAST>()},
        {"else", make_shared<BooleansTrueAST>()},
    };

}

