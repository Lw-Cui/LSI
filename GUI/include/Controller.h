#ifndef GI_TEXTWINDOW_H
#define GI_TEXTWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <string>
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

    class VertexArray : public sf::VertexArray {
    public:
        bool operator==(const VertexArray &va) const {
            if (va.getVertexCount() != getVertexCount()) return false;
            for (size_t i = 0; i < getVertexCount(); i++)
                if (va[i].position != operator[](i).position || va[i].color != operator[](i).color)
                    return false;
            return true;
        }
    };

    class Window {
    public:
        Window(const sf::VideoMode &vm, const std::string &str) :
                winPtr(std::make_shared<sf::RenderWindow>(vm, str)) {}

        Window() : winPtr(nullptr) {}

        virtual void draw(VertexArray va) {
            if (winPtr) {
                for (int i = 0; i < va.getVertexCount(); i++) {
                    /*
                        va[i].position.x += getSize().x / 2;
                        va[i].position.y = -va[i].position.y + getSize().y / 2;
                    */
                    va[i].position.y = getSize().y - va[i].position.y;
                }
                winPtr->draw(va);
            }
        }

        virtual void draw(const Text &text) { if (winPtr)winPtr->draw(text); }

        void clear(sf::Color c) { if (winPtr) winPtr->clear(c); }

        sf::Vector2u getSize() const { if (winPtr) return winPtr->getSize(); else return sf::Vector2u{0, 0}; }

        void setPosition(sf::Vector2i pos) { if (winPtr) winPtr->setPosition(pos); }

        void setVerticalSyncEnabled(bool status) { if (winPtr) winPtr->setVerticalSyncEnabled(status); }

        bool isOpen() const { if (winPtr) return winPtr->isOpen(); else return true; }

        bool pollEvent(sf::Event &event) {
            if (winPtr) return winPtr->pollEvent(event);
            else return false;
        }

        void close() { if (winPtr) winPtr->close(); }

        void display() { if (winPtr) winPtr->display(); }

    private:
        std::shared_ptr<sf::RenderWindow> winPtr;
    };

    class Controller {
    public:
        Controller(Window &text, Window &board);

        void drawToWindows();

        void appendChar(char);

        void appendShape(const VertexArray &);

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
        void adjustText();

        void lineFeedProcess();

        void backSpaceProcess();

        void normalCharProcess(char c);

        Text evaluation();

        Window &textWindow;
        mutable Text currentText;
        std::vector<Text> history;
        context::pScope scope;

        Window &drawingBoard;
        std::vector<con::VertexArray> shapes;

    };
}

#endif //GI_TEXTWINDOW_H
