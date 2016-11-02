#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main() {
    sf::RenderWindow drawingBoard(sf::VideoMode(1300, 1300), "Drawing Board"),
            textWindow(sf::VideoMode(1100, 800), "Shell");
    drawingBoard.setPosition(sf::Vector2i{1100, 0});
    textWindow.setPosition(sf::Vector2i{0, 0});
    drawingBoard.setVerticalSyncEnabled(true);
    textWindow.setVerticalSyncEnabled(true);

    std::string s;

    sf::Font font;
    font.loadFromFile("monaco.ttf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(26);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    while (textWindow.isOpen() && drawingBoard.isOpen()) {
        sf::Event event;

        while (textWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            } else if (event.type == sf::Event::TextEntered) {
                s.push_back(static_cast<char>(event.text.unicode));
                text.setString(s);
            }
        }
        while (drawingBoard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            }
        }

        textWindow.clear(sf::Color::White);
        drawingBoard.clear(sf::Color::White);
        textWindow.draw(text);

        textWindow.display();
        drawingBoard.display();
    }
    return 0;
}

