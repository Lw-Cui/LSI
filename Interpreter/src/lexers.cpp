#include <lexers.h>

using namespace lexers;

Lexer &Lexer::appendExp(const std::string &exp) {
    // Clear EOF, if stream has
    if (getTokType() == TokEOF) expressionBuf.clear();
    expressionBuf << "\n" << processExp(exp);
    if (getTokType() == TokEOF) stepForward();
    return *this;
}

double Lexer::getNum() {
    if (getTokType() == TokNumber) {
        auto tmp = numToken;
        stepForward();
        return tmp;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't number.");
    }
}

std::string Lexer::getIdentifier() {
    if (getTokType() == TokIdentifier) {
        std::string tmp = strToken;
        stepForward();
        return std::move(tmp);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Token isn't identifier.");
    }
}

Lexer::TokenType Lexer::getTokType() const {
    return currentType;
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
        Number:
        currentType = TokNumber;
    } else if (type == '(') {
        expressionBuf.get();
        currentType = TokOpeningBracket;
    } else if (type == ')') {
        expressionBuf.get();
        currentType = TokClosingBracket;
    } else {
        expressionBuf >> strToken;
        if (strToken.size() > 1
            && (strToken[0] == '+' || strToken[0] == '-')
            && isdigit(strToken[1])) {
            std::stringstream(strToken) >> numToken;
            goto Number;
        }
        currentType = (keyWord.count(strToken) ? keyWord[strToken] : TokIdentifier);
        if (strToken == "#") {
            do {
                type = expressionBuf.get();
            } while (type != EOF && type != '\n');
            return stepForward();
        }
    }
    return currentType;
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

void Lexer::clear() {
    expressionBuf.str("");
    if (currentType == TokEOF) expressionBuf.clear();
    currentType = TokEOF;
}


