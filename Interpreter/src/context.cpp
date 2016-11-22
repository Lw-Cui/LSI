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
            impl["null?"] = make_shared<BuiltinNullAST>();
            impl["<"] = make_shared<BuiltinLessThanAST>();
            impl["~"] = make_shared<BuiltinMinusSignAST>();
            impl["list"] = make_shared<BuiltinListAST>();
        }

        shared_ptr<parser::ExprAST> &operator[](const std::string &str) {
            return impl[str];
        }

        size_t count(const std::string &str) const {
            return impl.count(str);
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
}

