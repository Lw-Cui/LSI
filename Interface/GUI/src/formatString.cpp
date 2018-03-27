#include <formatString.h>

using namespace std;
using namespace con;

void con::FormatString::lineFeedProcess() {
    if (!openBracketPos.empty())
        content.push_back(make_pair(openBracketPos.top(), string()));
    else
        content.push_back(make_pair(0, string()));
}

void con::FormatString::backSpaceProcess() {
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

void con::FormatString::normalCharProcess(char c) {
    content.back().second.push_back(c);
    if (c == '(') {
        openBracketPos.push(content.back().first + content.back().second.size() + 1);
    } else if (c == ')' && !openBracketPos.empty()) {
        delBracketPos.push(openBracketPos.top());
        openBracketPos.pop();
    }
}

std::string con::FormatString::toString() const {
    string str;
    for (auto &s: content) {
        str += string(s.first, ' ') + s.second + "\n";
    }
    return std::move(str);
}

con::FormatString::FormatString() : content{1} {
}

void con::FormatString::clearStr() {
    content.clear();
    content.push_back(make_pair(0, string{}));
    openBracketPos = std::stack<unsigned long>();
    delBracketPos = std::stack<unsigned long>();
}

void con::pushString(FormatString &formatString, const std::string &str) {
    for (char c: str)
        formatString.normalCharProcess(c);
}

void con::setLineFeed(FormatString &formatString, int count) {
    for (int i = 0; i < count; i++)
        formatString.lineFeedProcess();
}

void con::setBackSpace(FormatString &formatString, int count) {
    for (int i = 0; i < count; i++)
        formatString.backSpaceProcess();
}
