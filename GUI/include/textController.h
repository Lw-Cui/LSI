#ifndef GI_TEXTWINDOW_H
#define GI_TEXTWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <formatString.h>
#include <context.h>
#include <parser.h>

namespace text {
    class Text : public sf::Drawable {
    public:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        Text(const std::string &fontFile = "../font/monaco.ttf") {
            font.loadFromFile(fontFile);
        }

        void clearStr();

        float getHeight() const;

        FormatString formatString;
        sf::Font font;
        sf::Color color = sf::Color::Black;
        unsigned int fontSize = 30;
        float offsetY = 0;
    };

    class TextController : public sf::Drawable {
    public:

        TextController(const sf::Vector2u &s) : screenSize{s} {
            lexers::Lexer lex("(load \"Base.scm\")");
            parser::parseAllExpr(lex)->eval(scope);
        }

        void appendChar(char);

        void moveScreen(float);

        void clearScreen();

        void execute();

        enum charType {
            BackSpace,
            LineFeed,
        };

        std::map<char, charType> toType = {
                {8,  BackSpace},
                {10, LineFeed},
        };

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        void lineFeedProcess();

        void backSpaceProcess();

        void normalCharProcess(char c);

        Text evaluation();

        context::Scope scope;
        std::vector<Text> history;
        mutable Text currentText;
        sf::Vector2u screenSize;
    };
}

#endif //GI_TEXTWINDOW_H
