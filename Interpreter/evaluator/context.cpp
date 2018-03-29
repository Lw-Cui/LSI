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

    class ScopeImpl {
    public:
        ScopeImpl() {
            addAllBuiltinFunc();
        }

        void clear() {
            impl.clear();
            addAllBuiltinFunc();
        }


        bool justCount(const std::string &id) const {
            return (bool)impl.count(id);
        }

        bool count(const std::string &str) const {
            return searchName(str) != nullptr;
        }

        void setDynamicScope(const std::shared_ptr<Scope> &scope) {
            dynamicScope = scope;
        }

        void setLexicalScope(const std::shared_ptr<Scope> &scope) {
            lexicalScope = scope;
        }

        bool addName(const std::string &id, pExpr ptr) {
            //if (impl.count(id)) return false;
            impl[id] = ptr;
            return true;
        }

        pExpr searchName(const std::string &id) const {
            if (impl.count(id)) return impl.find(id)->second;

            pExpr ret = nullptr;
            if (lexicalScope && ((ret = lexicalScope->searchName(id)) != nullptr))
                return ret;
            if (dynamicScope && ((ret = dynamicScope->searchName(id)) != nullptr))
                return ret;
            return ret;
        }

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) {
            impl[name] = expr;
        }

        /*
        const string getCurFuncName() const {
            if (!callStack.empty()) return callStack.top();
            // That is necessary since in function body we need new scope

            if (!nameDomain.empty()) {
                auto name = nameDomain.back()->getCurFuncName();
                if (!name.empty()) return name;
            }
            return "";
        }

        void setCurFuncName(const string &name) {
            if (!builtinList.count(name)) callStack.push(name);
        }

        bool delCurFuncName() {
            if (!callStack.empty()) {
                callStack.pop();
                return true;
            } else {
                if (!nameDomain.empty())
                    return nameDomain.back()->delCurFuncName();
                return false;
            }
        }
         */

        void addAllBuiltinFunc() {
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


        std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> impl;

        pScope dynamicScope, lexicalScope;

        static std::stack<std::string> callStack;

        const std::set<std::string> builtinList = {
            "cons",
            "car",
            "cdr",
            "+",
            "*",
            "null?",
            "<",
            "#opposite",
            "#reciprocal",
            "list",
            "else",
        };
    };

    std::stack<std::string> ScopeImpl::callStack;

    bool Scope::count(const std::string &str) const {
        return impl->count(str);
    }

    bool Scope::justCount(const std::string &str) const {
        return impl->justCount(str);
    }

    Scope::Scope() : impl{new ScopeImpl} {}

    void Scope::clear() { impl->clear(); }

    void Scope::addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) const {
        impl->addBuiltinFunc(name, expr);
    }

    pExpr Scope::searchName(const std::string &id) const {
        return impl->searchName(id);
    }

    bool Scope::addName(const std::string &id, pExpr ptr) {
        return impl->addName(id, ptr);
    }

    void Scope::openNewScope(pScope &p) {
        auto tmp = p;
        p = make_shared<Scope>();
        p->setLexicalScope(tmp);
    }

    Scope::Scope(const Scope &other) : impl(new ScopeImpl(*other.impl)) {
    }

    /*
    const std::string Scope::getCurFuncName() const {
        return impl->getCurFuncName();
    }

    void Scope::setCurFuncName(const std::string &str) {
        impl->setCurFuncName(str);
    }

    bool Scope::delCurFuncName() {
        return impl->delCurFuncName();
    }
     */

    void Scope::setLexicalScope(const std::shared_ptr<Scope> &scope) {
        impl->setLexicalScope(scope);
    }

    void Scope::setDynamicScope(const std::shared_ptr<Scope> &scope) {
        impl->setDynamicScope(scope);
    }

}

