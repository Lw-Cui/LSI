#ifndef GI_LEXERS_H
#define GI_LEXERS_H

#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <easylogging++.h>

namespace lexers {
    class Lexer {
    public:
        enum TokenType {
            TokEOF = -1,
            TokNumber = -2,
            TokIdentifier = -3,
            TokOpenBrace = -4,
            TokCloseBrace = -5,
            TokDefine = -6,
            TokLet = -7,
        };

        void appendExp(const std::string &exp) {
            expressionBuf.clear();
            expressionBuf << processExp(exp);
            CLOG(DEBUG, "lexer") << "String is:" << exp;
            if (getTokType() == TokEOF) getNextTok();
        }

        Lexer(const std::string &exp) {
            appendExp(exp);
        }

        TokenType getTokType() const { return currentType; }

        double getNum() {
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

        std::string getIdentifier() {
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

        TokenType getNextTok() {
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
            } else if (type == '(') {
                expressionBuf.get();
                currentType = TokOpenBrace;
            } else if (type == ')') {
                expressionBuf.get();
                currentType = TokCloseBrace;
            }
            return currentType;
        }

    private:
        std::string processExp(const std::string exp) const {
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

        std::string identifier;
        double number;

        std::stringstream expressionBuf;
        TokenType currentType = TokEOF;
    };
}
#endif //GI_LEXERS_H
