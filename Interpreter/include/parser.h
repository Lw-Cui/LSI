
#ifndef GI_PARSER_H
#define GI_PARSER_H

#include <lexers.h>
#include <AST.h>

namespace parser {

    std::shared_ptr<ExprAST> parseExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseNumberExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseOperatorExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseAddOperatorExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLessThanOperatorExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdentifierExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIfStatementExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLambdaApplicationExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionApplicationExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseLetExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseIdDefinitionExpr(lexers::Lexer &lex);

    std::shared_ptr<ExprAST> parseFunctionDefinitionExpr(lexers::Lexer &lex);
}
#endif //GI_PARSER_H
