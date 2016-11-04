#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
namespace {
    sf::Font font;
};

sf::Text getText(const std::string &res, sf::Color c = sf::Color::Black) {
    sf::Text text;
    text.setString(res);
    text.setFont(font);
    text.setCharacterSize(27);
    text.setFillColor(c);
    return std::move(text);
}

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    sf::RenderWindow drawingBoard(sf::VideoMode(1300, 1300), "Drawing Board"),
            textWindow(sf::VideoMode(1100, 800), "Shell");
    drawingBoard.setPosition(sf::Vector2i{1100, 0});
    textWindow.setPosition(sf::Vector2i{0, 0});
    drawingBoard.setVerticalSyncEnabled(true);
    textWindow.setVerticalSyncEnabled(true);

    font.loadFromFile("monaco.ttf");
    vector<sf::Text> history;

    std::string s;
    sf::Text currentText{getText(s)};

    while (textWindow.isOpen() && drawingBoard.isOpen()) {
        sf::Event event;
        textWindow.clear(sf::Color::White);
        drawingBoard.clear(sf::Color::White);

        while (textWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (isprint(event.text.unicode)) {
                    s.push_back(static_cast<char>(event.text.unicode));
                } else if (event.text.unicode == 8 && !s.empty()) { // BackSpace
                    s.pop_back();
                } else if (event.text.unicode == 10) { // LineFeed
                    s += "\n\t";
                    long openBrace = std::count(begin(s), end(s), ')');
                    long closeBrace = std::count(begin(s), end(s), '(');
                    if (openBrace > 0 && openBrace == closeBrace) {
                        history.push_back(currentText);
                        history.back().setFillColor(sf::Color::Red);
                        float delta = 10 + currentText.getLocalBounds().height;
                        for_each(begin(history), end(history),
                                 [delta](sf::Text &text) { text.move(0, -delta); });
                        s.clear();
                    }
                }
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                auto delta = event.mouseWheelScroll.delta * 5;
                currentText.move(0, delta);
                for_each(begin(history), end(history),
                         [delta](sf::Text &text) { text.move(0, delta); });
            }
        }
        while (drawingBoard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            }
        }

        currentText.setString(s);
        textWindow.draw(currentText);
        for_each(begin(history), end(history),
                 [&textWindow](const sf::Text &text) { textWindow.draw(text); });

        textWindow.display();
        drawingBoard.display();
    }
    return 0;
}

