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
            TokOperator = -8,
        };

        void appendExp(const std::string &exp);

        Lexer(const std::string &exp) {
            appendExp(exp);
        }

        TokenType getTokType() const { return currentType; }

        double getNum();

        std::string getOperator();

        std::string getIdentifier();

        TokenType getNextTok();

    private:
        bool isOperator(int type) const;

        std::string processExp(const std::string exp) const;

        std::string identifier;
        double number;

        std::stringstream expressionBuf;
        TokenType currentType = TokEOF;
    };
}
#endif //GI_LEXERS_H
