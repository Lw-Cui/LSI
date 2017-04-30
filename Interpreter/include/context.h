#ifndef GI_CONTEXT_H
#define GI_CONTEXT_H

#include <memory>
#include <string>
#include <unordered_map>

namespace ast {
    class ExprAST;

}

namespace context {

    using Iter = std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>>::const_iterator;
    using pExpr = std::shared_ptr<ast::ExprAST>;

    class ScopeImpl;

    class Scope {
        friend class ScopeImpl;

    public:
        Scope();

        Scope(std::shared_ptr<ScopeImpl>);

        bool addName(const std::string &id, pExpr ptr);

        pExpr searchName(const std::string &) const;

        void setSearchDomain(const std::shared_ptr<Scope> &);

        bool count(const std::string &str) const;

        void openNewScope(std::shared_ptr<Scope> &);

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &) const;

        void clear();

    private:

        std::shared_ptr<ScopeImpl> impl;

    };

    using pScope = std::shared_ptr<Scope>;
}

#endif //GI_CONTEXT_H
