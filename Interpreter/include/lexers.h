#ifndef GI_LEXERS_H
#define GI_LEXERS_H

#include <set>
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
            TokOpeningBracket = -4,
            TokClosingBracket = -5,
            TokDefine = -6,
            TokLet = -7,
            TokIf = -8,
            TokTrue = -9,
            TokFalse = -10,
            TokLoad = -11,
            TokLambda = -12,
            TokNil = -13,
            TokCond = -14,
        };


        virtual Lexer &appendExp(const std::string &exp);

        Lexer() {}

        Lexer(const std::string &exp) {
            appendExp(exp);
        }

        virtual TokenType getTokType() const;

        virtual double getNum();

        virtual std::string getIdentifier();

        TokenType stepForward();

    private:

        std::string processExp(const std::string exp) const;

        std::map<std::string, TokenType> keyWord = {
                {"define", TokDefine},
                {"let",    TokLet},
                {"if",     TokIf},
                {"cond",   TokCond},
                {"let",    TokLet},
                {"#t",     TokTrue},
                {"#f",     TokFalse},
                {"load",   TokLoad},
                {"lambda", TokLambda},
                {"nil",    TokNil},
        };

        std::string strToken;
        double numToken;

        std::stringstream expressionBuf;
        TokenType currentType = TokEOF;
    };
}
#endif //GI_LEXERS_H
