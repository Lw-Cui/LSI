#include <lexers.h>
#include <parser.h>
using namespace lexers;
using namespace parser;
using namespace std;

ASTPtr parser::parseExpr(lexers::Lexer &lex) {
    switch(lex.getTokType()) {
        case Lexer::TokNumber:
            return parseNumberExpr(lex);
    }
}

ASTPtr parser::parseNumberExpr(Lexer &lex) {
    return make_shared<NumberAST>(lex.getNum());
}
