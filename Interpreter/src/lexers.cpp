#include <lexers.h>

using namespace lexers;

void Lexer::appendExp(const std::string &exp) {
    expressionBuf.clear();
    expressionBuf << processExp(exp);
    CLOG(DEBUG, "lexer") << "String is:" << exp;
    if (getTokType() == TokEOF) getNextTok();
}

double Lexer::getNum() {
    if (getTokType() == TokNumber) {
        auto tmp = number;
        CLOG(DEBUG, "lexer") << "Get number: " << tmp;
        getNextTok();
        return tmp;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't number.");
    }
}

std::string Lexer::getOperator() {
    if (getTokType() == TokOperator) {
        std::string tmp = identifier;
        CLOG(DEBUG, "lexer") << "Get Operator: " << tmp;
        getNextTok();
        return std::move(tmp);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't Operator.");
    }
}

std::string Lexer::getIdentifier() {
    if (getTokType() == TokIdentifier) {
        std::string tmp = identifier;
        CLOG(DEBUG, "lexer") << "Get identifier: " << tmp;
        getNextTok();
        return std::move(tmp);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't identifier.");
    }
}

Lexer::TokenType Lexer::getNextTok() {
    int type{expressionBuf.get()};
    expressionBuf.unget();

    if (type == EOF) {
        currentType = TokEOF;
    } else if (isspace(type)) {
        expressionBuf.get();
        return getNextTok();
    } else if (isdigit(type)) {
        expressionBuf >> number;
        CLOG(DEBUG, "lexer") << "Read number: " << number;
        currentType = TokNumber;
    } else if (isalpha(type)) {
        expressionBuf >> identifier;
        CLOG(DEBUG, "lexer") << "Read identifier: " << identifier;
        if (identifier == "define")
            currentType = TokDefine;
        else if (identifier == "let")
            currentType = TokLet;
        else
            currentType = TokIdentifier;
    } else if (isOperator(type)) {
        expressionBuf >> identifier;
        CLOG(DEBUG, "lexer") << "Read operator: " << identifier;
        currentType = TokOperator;
    } else if (type == '(') {
        expressionBuf.get();
        currentType = TokOpenBrace;
    } else if (type == ')') {
        expressionBuf.get();
        currentType = TokCloseBrace;
    }
    return currentType;
}


bool Lexer::isOperator(int type) const {
    switch (type) {
        case '+':
        case '-':
        case '>':
        case '<':
            return true;
        default:
            return false;
    }
}

std::string Lexer::processExp(const std::string exp) const {
    std::string tmp;
    std::for_each(std::begin(exp), std::end(exp), [&tmp](const char c) {
        if (c == ')' || c == '(') {
            tmp.push_back(' ');
            tmp.push_back(c);
            tmp.push_back(' ');
        } else {
            tmp.push_back(c);
        }
    });
    return std::move(tmp);
}

