#ifndef GI_TEXTWINDOW_H
#define GI_TEXTWINDOW_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class TextWindow : public sf::Drawable {
public:

    TextWindow();

    void appendChar(char);

    void move(float);

    enum charType {
        BackSpace,
        LineFeed,
    };

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    std::map<char, charType> toType = {
            {8,  BackSpace},
            {10, LineFeed},
    };

    void lineFeedProcess();

    void backSpaceProcess();

    void normalCharProcess(char c);

    std::vector<sf::Text> history;

    std::string context;
    mutable sf::Text currentText;

    sf::Font font;
};

#endif //GI_TEXTWINDOW_H
