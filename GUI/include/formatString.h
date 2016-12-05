#ifndef GI_UTILITY_H
#define GI_UTILITY_H

#include <string>
#include <stack>
#include <vector>

namespace text {
    class FormatString {
    public:
        FormatString() {}

        void lineFeedProcess();

        void backSpaceProcess();

        void normalCharProcess(char c);

        std::string toString() const;

    private:
        std::vector<std::string> content;
    };
}

#endif //GI_UTILITY_H
