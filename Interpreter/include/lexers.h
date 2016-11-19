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
            TokIf = -9,
            TokTrue = -10,
            TokFalse = -11,
            TokLoad = -12,
            TokLambda = -13,
            TokBuiltinFunc = -14,
            TokNil = -15,
        };


        Lexer &appendExp(const std::string &exp);

        Lexer() {}

        Lexer(const std::string &exp) {
            appendExp(exp);
        }

        TokenType getTokType() const { return currentType; }

        double getNum();

        std::string getOperator();

        std::string getIdentifier();

        std::string getBuiltinFunc();

        TokenType stepForward();

    private:
        bool isOperator(int type) const;

        std::string processExp(const std::string exp) const;

        std::map<std::string, TokenType> keyWord = {
                {"define", TokDefine},
                {"let",    TokLet},
                {"if",     TokIf},
                {"#t",     TokTrue},
                {"#f",     TokFalse},
                {"load",   TokLoad},
                {"lambda", TokLambda},
                {"nil",    TokNil},
        };

        std::set<std::string> builtinFunc = {
                {"cons"},
                {"car"},
                {"cdr"},
                {"list"},
                {"null?"},
                {"+"},
                {"-"},
                {"<"},
        };

        std::string strToken;
        double numToken;

        std::stringstream expressionBuf;
        TokenType currentType = TokEOF;
    };
}
#endif //GI_LEXERS_H
