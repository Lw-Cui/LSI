#ifndef GI_CONTEXT_H
#define GI_CONTEXT_H

#include <memory>
#include <string>
#include <stack>
#include <unordered_map>
#include <set>

namespace ast {
    class ExprAST;

}

namespace context {

    using Iter = std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>>::const_iterator;

    using pExpr = std::shared_ptr<ast::ExprAST>;

    class Scope {
    public:
        Scope();

        bool addName(const std::string &id, pExpr ptr);

        pExpr searchName(const std::string &) const;

        void setDynamicScope(const std::shared_ptr<Scope> &);

        void setLexicalScope(const std::shared_ptr<Scope> &);

        bool count(const std::string &str) const;

        void clear();

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &);

        /*
        const std::string getCurFuncName() const;

        void setCurFuncName(const std::string &str);

        bool delCurFuncName();
         */
    private:

        void addAllBuiltinFunc();

        std::shared_ptr<Scope> dynamicScope, lexicalScope;

        static std::stack<std::string> callTrace;

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

        std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> symtab;
    };

    using pScope = std::shared_ptr<Scope>;
}

#endif //GI_CONTEXT_H
