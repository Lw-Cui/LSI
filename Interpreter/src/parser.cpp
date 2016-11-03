#include <lexers.h>
#include <parser.h>
#include <stdexcept>

using namespace lexers;
using namespace parser;
using namespace std;

ASTPtr parser::parseExpr(lexers::Lexer &lex) {
    switch (lex.getTokType()) {
        case Lexer::TokNumber:
            return parseNumberExpr(lex);
        case Lexer::TokIdentifier:
            return parseIdentifierExpr(lex);
        default:;
    }
}

ASTPtr parser::parseNumberExpr(Lexer &lex) {
    return make_shared<NumberAST>(lex.getNum());
}

ASTPtr parser::parseIdentifierExpr(lexers::Lexer &lex) {
    return make_shared<IdentifierAST>(lex.getIdentifier());
}
