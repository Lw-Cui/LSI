#include <formatString.h>

using namespace std;

void text::FormatString::lineFeedProcess() {
    if (!openBracketPos.empty())
        content.push_back(make_pair(openBracketPos.top(), string()));
    else
        content.push_back(make_pair(0, string()));
}

void text::FormatString::backSpaceProcess() {
    if (content.empty()) return;
    if (!content.back().second.empty()) {
        char c = content.back().second.back();
        content.back().second.pop_back();
        if (c == '(' && !openBracketPos.empty()) {
            delBracketPos.push(openBracketPos.top());
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
