#include <memory>
#include <unordered_map>
#include <AST.h>
#include <parser.h>
#include <context.h>

using namespace std;
using namespace ast;

namespace context {

    class ScopeImpl {
    public:
        ScopeImpl() {
            addAllBuiltinFunc();
        }

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) {
            impl[name] = expr;
        }

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

        void clear() {
            impl.clear();
            addAllBuiltinFunc();
        }

        bool count(const std::string &str) const {
            return searchName(str) != nullptr;
        }

        void setSearchDomain(const pScope &s) {
            nameDomain.push_back(s);
        }

        bool addName(const std::string &id, pExpr ptr) {
            if (impl.count(id)) return false;
            impl[id] = ptr;
            return true;
        }

        pExpr searchName(const std::string &id) const {
            if (impl.count(id)) return impl.find(id)->second;
            pExpr ret = nullptr;
            for (const auto &sptr: nameDomain)
                if ((ret = sptr->searchName(id)) != nullptr)
                    return ret;
            return ret;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> impl;

        std::vector<pScope> nameDomain;
    };

    bool Scope::count(const std::string &str) const {
        return impl->count(str);
    }

    Scope::Scope() : impl{new ScopeImpl} {}

    void Scope::clear() { impl->clear(); }

    void Scope::addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) const {
        impl->addBuiltinFunc(name, expr);
    }

    void Scope::setSearchDomain(const pScope &s) {
        impl->setSearchDomain(s);
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
        p->setSearchDomain(tmp);
    }

    Scope::Scope(std::shared_ptr<ScopeImpl> ptr) : impl(ptr) {
    }
}

