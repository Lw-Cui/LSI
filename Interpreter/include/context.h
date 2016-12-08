#ifndef GI_CONTEXT_H
#define GI_CONTEXT_H

#include <memory>
#include <string>
#include <unordered_map>

namespace ast {
    class ExprAST;
}

namespace context {
    class ScopeImpl;

    typedef std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>>::const_iterator Iter;

    class Scope {
    public:
        Scope();

        Scope(const Scope &);

        Scope &operator=(const Scope &);

        std::shared_ptr<ast::ExprAST> &operator[](const std::string &str);

        size_t count(const std::string &str) const;

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &) const;

        void clear();

        Iter begin() const;

        Iter end() const;

    private:
        std::shared_ptr<ScopeImpl> impl;
    };
}

#endif //GI_CONTEXT_H
