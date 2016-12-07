#include <stack>
#include <string>
#include <textController.h>
#include <context.h>
#include <parser.h>

using namespace std;
using namespace sf;
using namespace text;
using namespace context;
using namespace parser;

void TextController::appendChar(char c) {
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

void TextController::lineFeedProcess() {
    currentText.formatString.lineFeedProcess();
    auto newline = 10 + currentText.fontSize;
    if (currentText.offsetY + currentText.getHeight() + newline > screenSize.y) {
        for_each(begin(history), end(history), [newline](Text &text) { text.offsetY -= newline; });
        currentText.offsetY -= newline;
    }
}

void TextController::backSpaceProcess() {
    currentText.formatString.backSpaceProcess();
}

void TextController::normalCharProcess(char c) {
    currentText.formatString.normalCharProcess(c);
}

void TextController::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(currentText);
    for_each(begin(history), end(history), [&target](const Text &text) { target.draw(text); });
}

void TextController::moveScreen(float delta) {
    currentText.offsetY += delta;
    for_each(begin(history), end(history), [delta](Text &text) { text.offsetY += delta; });
}

void TextController::clearScreen() {
    history.clear();
    currentText.clearStr();
    scope.clear();
}

void TextController::execute() {
    history.push_back(currentText);
    history.push_back(evaluation());

    currentText.offsetY = history.back().offsetY + history.back().getHeight();
    currentText.clearStr();
}

text::Text TextController::evaluation() {
    Text resultText;
    resultText.color = sf::Color::Blue;
    resultText.offsetY = currentText.offsetY + currentText.getHeight();
    pushString(resultText.formatString, "-> ");

    lexers::Lexer lex(currentText.formatString.toString());
    if (auto ptr = parseAllExpr(lex)->eval(scope)) {
        pushString(resultText.formatString, ptr->display());
    } else {
        pushString(resultText.formatString, "\'()");
    }

    return resultText;
}

void text::Text::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text text{formatString.toString(), font, fontSize};
    text.setFillColor(color);
    text.move(0, offsetY);
    target.draw(text);
}

float text::Text::getHeight() const {
    return sf::Text{formatString.toString(), font, fontSize}.getLocalBounds().height;
}

void text::Text::clearStr() {
    formatString.clearStr();
}
