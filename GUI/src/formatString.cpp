#include <formatString.h>

using namespace std;
using namespace text;

void text::FormatString::lineFeedProcess() {
    if (!openBracketPos.empty())
        content.push_back(make_pair(openBracketPos.top(), string()));
    else
        content.push_back(make_pair(0, string()));
}

void text::FormatString::backSpaceProcess() {
    if (content.empty()) return;
    auto &str = content.back().second;
    if (!str.empty()) {
        char c = str.back();
        str.pop_back();
        if (c == '(') {
            openBracketPos.pop();
        } else if (c == ')' && !delBracketPos.empty()) {
            openBracketPos.push(delBracketPos.top());
            delBracketPos.pop();
        }
    } else if (content.size() > 1) {
        content.pop_back();
    }
}

void text::FormatString::normalCharProcess(char c) {
    content.back().second.push_back(c);
    if (c == '(') {
        openBracketPos.push(content.back().first + content.back().second.size());
    } else if (c == ')' && !openBracketPos.empty()) {
        delBracketPos.push(openBracketPos.top());
        openBracketPos.pop();
    }
}

std::string text::FormatString::toString() const {
    string str;
    for (auto &s: content) {
        str += string(s.first, ' ') + s.second + "\n";
    }
    return std::move(str);
}

text::FormatString::FormatString() : content{1} {
}

void text::FormatString::clearStr() {
    content.clear();
    content.push_back(make_pair(0, string{}));
    openBracketPos = std::stack<unsigned long>();
    delBracketPos = std::stack<unsigned long>();
}

void text::pushString(text::FormatString &formatString, const std::string &str) {
    for (char c: str)
        formatString.normalCharProcess(c);
}

void text::setLineFeed(text::FormatString &formatString, int count) {
    for (int i = 0; i < count; i++)
        formatString.lineFeedProcess();
}

void text::setBackSpace(text::FormatString &formatString, int count) {
    for (int i = 0; i < count; i++)
        formatString.backSpaceProcess();
}
