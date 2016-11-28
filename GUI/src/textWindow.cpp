#include <textWindow.h>

using namespace std;
using namespace sf;
using namespace tw;

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
    long openBrace = std::count(begin(currentText.context), end(currentText.context), ')');
    long closeBrace = std::count(begin(currentText.context), end(currentText.context), '(');
    if (openBrace > 0 && openBrace == closeBrace) {
        history.push_back(currentText);
        history.back().color = sf::Color::Red;
        float delta = 10 + currentText.getHeight();
        for_each(begin(history), end(history), [delta](Text &text) { text.offsetY -= delta; });
        currentText.context.clear();
    } else {
        currentText.context += "\n\t";
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


void tw::Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text text{context, font, size};
    text.setFillColor(color);
    text.move(0, offsetY);
    target.draw(text);
}

float tw::Text::getHeight() const {
    return sf::Text{context, font, size}.getLocalBounds().height;
}
