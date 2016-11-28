#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <easylogging++.h>
#include <textWindow.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace tw;

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    sf::RenderWindow drawingBoard(sf::VideoMode(1300, 1300), "Drawing Board"),
            textWindow(sf::VideoMode(1100, 800), "Shell");

    drawingBoard.setPosition(sf::Vector2i{1100, 0});
    textWindow.setPosition(sf::Vector2i{0, 0});

    drawingBoard.setVerticalSyncEnabled(true);
    textWindow.setVerticalSyncEnabled(true);

    TextWindow textControl;

    while (textWindow.isOpen() && drawingBoard.isOpen()) {
        sf::Event event;
        textWindow.clear(sf::Color::White);
        drawingBoard.clear(sf::Color::White);

        while (textWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            } else if (event.type == sf::Event::TextEntered) {
                textControl.appendChar(static_cast<char>(event.text.unicode));
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                textControl.move(event.mouseWheelScroll.delta * 5);
            }
        }

        while (drawingBoard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            }
        }

        textWindow.draw(textControl);

        textWindow.display();
        drawingBoard.display();
    }
    return 0;
}

