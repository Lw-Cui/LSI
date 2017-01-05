#ifndef GI_EXCEPTION_H
#define GI_EXCEPTION_H

#include <string>
#include <vector>

namespace exception {
    class RuntimeError {
    private:
        std::string errorInfo;
    public:
        RuntimeError(const std::string &what) { appendInfo(what); }

        void appendInfo(const std::string &what) {
            errorInfo += "\n\t" + what;
        }

        std::string what() const {
            return errorInfo;
        }
    };

    class DefinitionError : public RuntimeError {
    public:
        explicit DefinitionError(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class RawExprError : public RuntimeError {
    public:
        explicit RawExprError(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };
}

#endif //GI_EXCEPTION_H
