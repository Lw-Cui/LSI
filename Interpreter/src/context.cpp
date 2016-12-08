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

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) {
            impl[name] = expr;
        }

        shared_ptr<parser::ExprAST> &operator[](const std::string &str) {
            return impl[str];
        }

        void clear() {
            impl.clear();
        }

        size_t count(const std::string &str) const {
            return impl.count(str);
        }

        Iter begin() const {
            return impl.begin();
        }

        Iter end() const {
            return impl.end();
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> impl;
    };

    shared_ptr<parser::ExprAST> &Scope::operator[](const std::string &str) {
        return impl->operator[](str);
    }

    size_t Scope::count(const std::string &str) const {
        return impl->count(str);
    }

    Scope::Scope() : impl{new ScopeImpl} {}

    Scope::Scope(const Scope &s) : impl{make_shared<ScopeImpl>(*s.impl)} {
    }

    Scope &Scope::operator=(const Scope &s) {
        impl = make_shared<ScopeImpl>(*s.impl);
        return *this;
    }

    Iter Scope::begin() const { return impl->begin(); }

    Iter Scope::end() const { return impl->end(); }

    void Scope::clear() { impl->clear(); }

    void Scope::addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &expr) const {
        impl->addBuiltinFunc(name, expr);
    }
}

