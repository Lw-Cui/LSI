#include <stack>
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
    if (currentText.offsetY + currentText.getHeight() + currentText.size + 10 > size.y) {
        float delta = 10 + currentText.size;
        for_each(begin(history), end(history), [delta](Text &text) { text.offsetY -= delta; });
        currentText.offsetY -= delta;
    }

    if (currentText.isFinished()) {
        history.push_back(currentText);
        history.back().color = sf::Color::Red;
        currentText.offsetY += currentText.getHeight() + 10;
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

bool tw::Text::isFinished() const {
    if (context.front() != '(' || context.back() != ')') return false;
    stack<char> s;
    for (auto ch: context)
        if (ch == '(') s.push(ch);
        else if (ch == ')') {
            if (s.empty()) return false;
            else s.pop();
        }
    return s.empty();
}
