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
            impl["cons"] = make_shared<BuiltinConsAST>();
            impl["car"] = make_shared<BuiltinCarAST>();
            impl["cdr"] = make_shared<BuiltinCdrAST>();
            impl["+"] = make_shared<BuiltinAddAST>();
            impl["*"] = make_shared<BuiltinMultiplyAST>();
            impl["null?"] = make_shared<BuiltinNullAST>();
            impl["<"] = make_shared<BuiltinLessThanAST>();
            impl["#opposite"] = make_shared<BuiltinOppositeAST>();
            impl["#reciprocal"] = make_shared<BuiltinReciprocalAST>();
            impl["list"] = make_shared<BuiltinListAST>();
        }

        shared_ptr<parser::ExprAST> &operator[](const std::string &str) {
            return impl[str];
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
}

