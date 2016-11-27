#include <textWindow.h>

using namespace std;
using namespace sf;

void TextWindow::appendChar(char c) {
    if (!toType.count(c))
        return normalCharProcess(c);
    switch (toType[c]) {
        case BackSpace:
            return backSpaceProcess();
        case LineFeed:
            return lineFeedProcess();
        default:;
    }
}


void TextWindow::lineFeedProcess() {
    context += "\n\t";
    long openBrace = std::count(begin(context), end(context), ')');
    long closeBrace = std::count(begin(context), end(context), '(');
    if (openBrace > 0 && openBrace == closeBrace) {
        history.push_back(currentText);
        history.back().setFillColor(sf::Color::Red);
        float delta = 10 + currentText.getLocalBounds().height;
        for_each(begin(history), end(history), [delta](sf::Text &text) { text.move(0, -delta); });
        context.clear();
    }
}

void TextWindow::backSpaceProcess() {
    if (!context.empty()) context.pop_back();
}

void TextWindow::normalCharProcess(char c) {
    context.push_back(c);
}

void TextWindow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    currentText.setString(context);
    target.draw(currentText);
    for_each(begin(history), end(history), [&target](const sf::Text &text) { target.draw(text); });
}

void TextWindow::move(float delta) {
    currentText.move(0, delta);
    for_each(begin(history), end(history), [delta](sf::Text &text) { text.move(0, delta); });
}

TextWindow::TextWindow() {
    font.loadFromFile("monaco.ttf");

    currentText.setFont(font);
    currentText.setCharacterSize(27);
    currentText.setFillColor(sf::Color::Black);
}
