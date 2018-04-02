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

        void addSymbol(const std::string &id, pExpr ptr);

        pExpr findSymbol(const std::string &) const;

        void setDynamicScope(const std::shared_ptr<Scope> &);

        void setLexicalScope(const std::shared_ptr<Scope> &);

        void stepIntoAnonymousFunc();

        void stepIntoFunc(const std::string &name);

        void stepOutFunc();

        std::string currentFunc() const;

        bool count(const std::string &str) const;

        void clearCurScope();

        void addBuiltinFunc(const std::string &name, const std::shared_ptr<ast::ExprAST> &);

        //private:

        std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> symtab;

        std::shared_ptr<Scope> dynamicScope, lexicalScope;

        static std::stack<std::string> callTrace;

        static int anonymousId;

        static const std::unordered_map<std::string, std::shared_ptr<ast::ExprAST>> builtinFunc;
    };

    using pScope = std::shared_ptr<Scope>;
}

#endif //GI_CONTEXT_H
