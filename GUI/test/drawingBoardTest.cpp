#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <formatString.h>
#include <Controller.h>

using namespace con;
using namespace std;
using namespace sf;

class MockWindow : public con::Window {
public:
    MockWindow(const VideoMode &vm, const string &str) : con::Window(vm, str) {}

    MOCK_METHOD1(draw, void(
            const con::VertexArray &va));
};

TEST(DrawingBoardTest, drawPointTest) {
    testing::StrictMock<MockWindow> drawingBoard(sf::VideoMode(1300, 1300), "Drawing Board");
    Window textWindow(sf::VideoMode(1100, 800), "Shell");

    con::VertexArray va;
    va.append(Vertex{Vector2f(5, 5), Color::Black});
    EXPECT_CALL(drawingBoard, draw(va)).Times(1);

    Controller controller{textWindow, drawingBoard};
    string code{"(draw (list (cons 5 5)))"};
    for (char c: code) controller.appendChar(c);
    controller.execute();
    controller.drawToWindows();
}