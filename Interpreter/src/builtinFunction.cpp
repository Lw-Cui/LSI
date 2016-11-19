#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;

std::shared_ptr<ExprAST> parser::parserBuiltinFunction(lexers::Lexer &lexer) {
    auto func = lexer.getBuiltinFunc();
    if (func == "cons")
        return parseBuiltinConsExpr(lexer);
    else if (func == "car")
        return parseBuiltinCarExpr(lexer);
    else if (func == "cdr")
        return parseBuiltinCdrExpr(lexer);
    else if (func == "null?")
        return parseBuiltinNullExpr(lexer);
    else if (func == "+")
        return parseBuiltinAddExpr(lexer);
    else if (func == "-")
        return parseBuiltinMinusExpr(lexer);
    else if (func == "<")
        return parseBuiltinLessThanExpr(lexer);
    else
        throw std::logic_error("Builtin function error.");
}

std::shared_ptr<ExprAST> parser::parseBuiltinConsExpr(lexers::Lexer &lexer) {
    CLOG(DEBUG, "parser");
    auto first = parseExpr(lexer);
    auto second = parseExpr(lexer);
    return std::make_shared<PairAST>(first, second);
}

std::shared_ptr<ExprAST> parser::parseBuiltinCarExpr(lexers::Lexer &lexer) {
    CLOG(DEBUG, "parser");
    auto pair = parseExpr(lexer);
    return std::make_shared<BuiltinCarAST>(pair);
}

std::shared_ptr<ExprAST> parser::parseBuiltinCdrExpr(lexers::Lexer &lexer) {
    CLOG(DEBUG, "parser");
    auto pair = parseExpr(lexer);
    return std::make_shared<BuiltinCdrAST>(pair);
}

std::shared_ptr<ExprAST> parser::parseBuiltinNullExpr(lexers::Lexer &lexer) {
    CLOG(DEBUG, "parser");
    auto pair = parseExpr(lexer);
    return std::make_shared<BuiltinNullAST>(pair);
}


std::shared_ptr<ExprAST> parser::parseBuiltinAddExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of add operands are: " << arguments.size();
    return make_shared<BuiltinAddAST>(arguments);
}

std::shared_ptr<ExprAST> parser::parseBuiltinMinusExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of minus operands are: " << arguments.size();
    return make_shared<BuiltinMinusAST>(arguments);
}

std::shared_ptr<ExprAST> parser::parseBuiltinLessThanExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of less than operands are: " << arguments.size();
    return make_shared<BuiltinLessThanAST>(arguments);
}
