#ifndef GI_CONTEXT_H
#define GI_CONTEXT_H

#include <memory>
#include <string>

namespace ast {
    class ExprAST;
}

namespace context {
    class ScopeImpl;


    class Scope {
    public:
        Scope();

        std::shared_ptr<ast::ExprAST> &operator[](const std::string &str);

        size_t count(const std::string &str) const;

    private:
        std::shared_ptr<ScopeImpl> impl;
    };
}

#endif //GI_CONTEXT_H
