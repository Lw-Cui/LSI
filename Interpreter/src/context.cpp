#include <parser.h>
#include <context.h>

using namespace std;

namespace context {
    class ScopeImpl {
    public:
        shared_ptr<parser::ExprAST> &operator[](const std::string &str) {
            return impl[str];
        }

        size_t count(const std::string &str)const {
            return impl.count(str);
        }

    private:
        std::map<std::string, std::shared_ptr<parser::ExprAST>> impl;
    };

    shared_ptr<parser::ExprAST> &Scope::operator[](const std::string &str) {
        return impl->operator[](str);
    }

    size_t Scope::count(const std::string &str)const {
        return impl->count(str);
    }

    Scope::Scope():impl{new ScopeImpl} {
    }
}

