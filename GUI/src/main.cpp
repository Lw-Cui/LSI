#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <easylogging++.h>
#include <Controller.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace con;

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

    Controller controller{textWindow, drawingBoard};

    while (textWindow.isOpen() && drawingBoard.isOpen()) {
        sf::Event event;

        while (textWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            } else if (event.type == sf::Event::KeyPressed && event.key.system) {
                if (event.key.code == sf::Keyboard::E) {
                    controller.clearScreen();
                } else if (event.key.code == sf::Keyboard::R) {
                    controller.execute();
                }
            } else if (event.type == sf::Event::TextEntered) {
                controller.appendChar(static_cast<char>(event.text.unicode));
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                controller.moveScreen(event.mouseWheelScroll.delta * 5);
            }
        }

        while (drawingBoard.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                textWindow.close();
            }
        }

        controller.drawToWindows();
        textWindow.display();
        drawingBoard.display();
    }
    return 0;
}

