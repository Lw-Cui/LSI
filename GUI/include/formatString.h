#ifndef GI_UTILITY_H
#define GI_UTILITY_H

#include <string>
#include <stack>
#include <vector>

namespace text {
    class FormatString {
    public:
        FormatString();

        void lineFeedProcess();

        void backSpaceProcess();

        void normalCharProcess(char c);

        std::string toString() const;

    private:
        std::vector<std::pair<unsigned long, std::string>> content;
        // openBracketPos.top(): the lastest open brakcet which doesn't be closed
        // delBracketPos.top(): the lastest open brakcet which is closed
        std::stack<unsigned long> openBracketPos, delBracketPos;
    };
}

#endif //GI_UTILITY_H
