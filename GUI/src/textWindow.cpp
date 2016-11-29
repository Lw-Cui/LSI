#include <stack>
#include <string>
#include <textWindow.h>
#include <context.h>
#include <parser.h>

using namespace std;
using namespace sf;
using namespace tw;
using namespace context;
using namespace parser;

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
    currentText.lindFeed();
    auto newline = 10 + currentText.fontSize;
    if (currentText.offsetY + currentText.getHeight() + newline > screenSize.y) {
        for_each(begin(history), end(history), [newline](Text &text) { text.offsetY -= newline; });
        currentText.offsetY -= newline;
    }
}

void TextWindow::backSpaceProcess() {
    if (!currentText.context.empty()) currentText.context.pop_back();
}

void TextWindow::normalCharProcess(char c) {
    currentText.context.push_back(c);
}

void TextWindow::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(currentText);
    for_each(begin(history), end(history), [&target](const Text &text) { target.draw(text); });
}

void TextWindow::move(float delta) {
    currentText.offsetY += delta;
    for_each(begin(history), end(history), [delta](Text &text) { text.offsetY += delta; });
}

void TextWindow::clear() {
    history.clear();
    currentText.context.clear();
    context.clear();
}

void TextWindow::execute() {
    history.push_back(currentText);
    history.back().color = sf::Color::Red;
    currentText.offsetY += currentText.getHeight() + 10;
    currentText.context.clear();
}

void tw::Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text text{context, font, fontSize};
    text.setFillColor(color);
    text.move(0, offsetY);
    target.draw(text);
}

float tw::Text::getHeight() const {
    return sf::Text{context, font, fontSize}.getLocalBounds().height;
}

void tw::Text::lindFeed() {
}

unsigned int tw::Text::getIndentation(size_t pos) const {
}
