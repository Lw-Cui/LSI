#ifndef GI_TEXTWINDOW_H
#define GI_TEXTWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <context.h>

namespace tw {
    class Text : public sf::Drawable {
    public:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        Text(const std::string &fontFile = "monaco.ttf") {
            font.loadFromFile(fontFile);
        }

        float getHeight() const;

        void lindFeed();

        std::string context;
        sf::Font font;

        sf::Color color = sf::Color::Black;
        unsigned int fontSize = 30;
        float offsetY = 0;

    private:
        unsigned int getIndentation(size_t pos) const;
    };

    class TextWindow : public sf::Drawable {
    public:

        TextWindow(const sf::Vector2u &s) : screenSize{s} {}

        void appendChar(char);

        void move(float);

        void clear();

        void execute();

        enum charType {
            BackSpace,
            LineFeed,
        };

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        std::map<char, charType> toType = {
                {8,  BackSpace},
                {10, LineFeed},
        };

        void lineFeedProcess();

        void backSpaceProcess();

        void normalCharProcess(char c);

        context::Scope context;
        std::vector<Text> history;
        mutable Text currentText;
        sf::Vector2u screenSize;
    };
}

#endif //GI_TEXTWINDOW_H
