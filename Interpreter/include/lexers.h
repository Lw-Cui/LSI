#ifndef GI_LEXERS_H
#define GI_LEXERS_H

#include <string>
#include <sstream>

namespace lexers {
    class Lexer {
    public:
        enum TokenType {
            TokEOF = -1,
            TokNumber = -2,
        };

        Lexer(const std::string &exp) : expressionBuf{exp} { getNextTok(); }

        TokenType getTokType() const { return currentType; }

        TokenType getNextTok() {
            int type{expressionBuf.get()};
            expressionBuf.unget();

            if (type == EOF) {
                currentType = TokEOF;
            } else if (isdigit(type)) {
                expressionBuf >> token.number;
                currentType = TokNumber;
            }
            return currentType;
        }

        double getNum() const { return token.number; }

        std::string getIdentifier() const { return token.identifier; }

    private:
        union Token {
            std::string identifier;
            double number;

            Token() {}

            ~Token() {}
        } token;

        std::stringstream expressionBuf;
        TokenType currentType = TokEOF;
    };
}
#endif //GI_LEXERS_H
