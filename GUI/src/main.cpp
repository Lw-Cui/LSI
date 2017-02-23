#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sys/resource.h>
#include <SFML/Graphics.hpp>
#include <easylogging++.h>
#include <Controller.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace con;

/*
void setStack(rlim_t stackSize) {
    struct rlimit rl;
    if (getrlimit(RLIMIT_STACK, &rl) == 0) {
        if (rl.rlim_cur < stackSize) {
            rl.rlim_cur = stackSize;
            if (setrlimit(RLIMIT_STACK, &rl) != 0) {
                CLOG(DEBUG, "exception");
                throw std::logic_error("Cannot set resource info.");
            }
        }
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Cannot get resource info.");
    }
}
*/

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger *parserLogger = el::Loggers::getLogger("parser");
    el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger] %msg [%fbase:%line]");

    //setStack(16 * 1024 * 1024);   // 16MB

    Window drawingBoard(sf::VideoMode(1000, 1000), "Drawing Board"),
            textWindow(sf::VideoMode(1000, 1000), "Shell");

    drawingBoard.setPosition(sf::Vector2i{1000, 0});
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

