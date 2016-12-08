#ifndef GI_TEXTWINDOW_H
#define GI_TEXTWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <formatString.h>
#include <context.h>
#include <parser.h>

namespace ast {
    class BuiltinDrawAST;
}

namespace con {
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

    class Controller {
    public:
        Controller(sf::RenderTarget &text, sf::RenderTarget &board);

        void drawToWindows();

        void appendChar(char);

        void appendShape(const sf::VertexArray &);

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

        sf::RenderTarget &textWindow;
        mutable Text currentText;
        std::vector<Text> history;
        context::Scope scope;

        sf::RenderTarget &drawingBoard;
        std::vector<sf::VertexArray> shapes;

    };
}

#endif //GI_TEXTWINDOW_H
