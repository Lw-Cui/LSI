#include <lexers.h>

using namespace lexers;

Lexer &Lexer::appendExp(const std::string &exp) {
    expressionBuf.clear();
    expressionBuf << processExp(exp);
    CLOG(DEBUG, "lexer") << "String is:" << exp;
    if (getTokType() == TokEOF) stepForward();
    return *this;
}

double Lexer::getNum() {
    if (getTokType() == TokNumber) {
        auto tmp = numToken;
        CLOG(DEBUG, "lexer") << "Get number: " << tmp;
        stepForward();
        return tmp;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't number.");
    }
}

std::string Lexer::getOperator() {
    if (getTokType() == TokOperator) {
        std::string tmp = strToken;
        CLOG(DEBUG, "lexer") << "Get Operator: " << tmp;
        stepForward();
        return std::move(tmp);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't Operator.");
    }
}

std::string Lexer::getIdentifier() {
    if (getTokType() == TokIdentifier) {
        std::string tmp = strToken;
        CLOG(DEBUG, "lexer") << "Get identifier: " << tmp;
        stepForward();
        return std::move(tmp);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't identifier.");
    }
}

Lexer::TokenType Lexer::stepForward() {
    int type{expressionBuf.get()};
    expressionBuf.unget();

    if (type == EOF) {
        currentType = TokEOF;
    } else if (isspace(type)) {
        expressionBuf.get();
        return stepForward();
    } else if (isdigit(type)) {
        expressionBuf >> numToken;
        CLOG(DEBUG, "lexer") << "Read number: " << strToken;
        currentType = TokNumber;
    } else if (isalpha(type) || type == '#') {
        expressionBuf >> strToken;
        CLOG(DEBUG, "lexer") << "Read string token: " << strToken;
        currentType = (keyWord.count(strToken) ? keyWord[strToken] : TokIdentifier);
    } else if (isOperator(type)) {
        expressionBuf >> strToken;
        CLOG(DEBUG, "lexer") << "Read operator: " << strToken;
        currentType = TokOperator;
    } else if (type == '(') {
        expressionBuf.get();
        currentType = TokOpeningBracket;
    } else if (type == ')') {
        expressionBuf.get();
        currentType = TokClosingBracket;
    }
    return currentType;
}


bool Lexer::isOperator(int type) const {
    switch (type) {
        case '+':
        case '-':
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

