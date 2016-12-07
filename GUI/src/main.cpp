#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <easylogging++.h>
#include <textController.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace text;

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger *parserLogger = el::Loggers::getLogger("parser");
    el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger] %msg [%fbase:%line]");

    sf::RenderWindow drawingBoard(sf::VideoMode(1300, 1300), "Drawing Board"),
            textWindow(sf::VideoMode(1100, 800), "Shell");

    drawingBoard.setPosition(sf::Vector2i{1100, 0});
    textWindow.setPosition(sf::Vector2i{0, 0});

    drawingBoard.setVerticalSyncEnabled(true);
    textWindow.setVerticalSyncEnabled(true);

    TextController textController{textWindow.getSize()};

    while (textWindow.isOpen() && drawingBoard.isOpen()) {
        sf::Event event;
        textWindow.clear(sf::Color::White);
        drawingBoard.clear(sf::Color::White);

        while (textWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            } else if (event.type == sf::Event::KeyPressed && event.key.system) {
                if (event.key.code == sf::Keyboard::E)
                    textController.clearScreen();
                else if (event.key.code == sf::Keyboard::R)
                    textController.execute();
            } else if (event.type == sf::Event::TextEntered) {
                textController.appendChar(static_cast<char>(event.text.unicode));
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                textController.moveScreen(event.mouseWheelScroll.delta * 5);
            }
        }

        while (drawingBoard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            }
        }

        textWindow.draw(textController);
        textWindow.display();
        drawingBoard.display();
    }
    return 0;
}

