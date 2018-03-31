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

    Scope::Scope() {
        addAllBuiltinFunc();
    }

    void Scope::clear() {
        symtab.clear();
        addAllBuiltinFunc();
    }

    void Scope::addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) {
        symtab[name] = expr;
    }

    pExpr Scope::findSymbol(const std::string &id) const {
        if (symtab.count(id)) return symtab.find(id)->second;

        pExpr ret = nullptr;
        if (lexicalScope && ((ret = lexicalScope->findSymbol(id)) != nullptr))
            return ret;
        if (dynamicScope && ((ret = dynamicScope->findSymbol(id)) != nullptr))
            return ret;
        return ret;
    }

    bool Scope::addSymbol(const std::string &id, pExpr ptr) {
        symtab[id] = ptr;
        return true;
    }

    void Scope::addAllBuiltinFunc() {
        addBuiltinFunc("cons", make_shared<BuiltinConsAST>());
        addBuiltinFunc("car", make_shared<BuiltinCarAST>());
        addBuiltinFunc("cdr", make_shared<BuiltinCdrAST>());
        addBuiltinFunc("+", make_shared<BuiltinAddAST>());
        addBuiltinFunc("*", make_shared<BuiltinMultiplyAST>());
        addBuiltinFunc("null?", make_shared<BuiltinNullAST>());
        addBuiltinFunc("<", make_shared<BuiltinLessThanAST>());
        addBuiltinFunc("#opposite", make_shared<BuiltinOppositeAST>());
        addBuiltinFunc("#reciprocal", make_shared<BuiltinReciprocalAST>());
        addBuiltinFunc("list", make_shared<BuiltinListAST>());
        addBuiltinFunc("else", make_shared<BooleansTrueAST>());
    }

    std::stack<std::string> Scope::callTrace;

    int Scope::anonymousId;


    void Scope::stepIntoFunc(const std::string &name) {
        if (builtinList.count(name)) return;
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

}

