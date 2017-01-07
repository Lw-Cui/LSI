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

    class NotAtomType : public RuntimeError {
    public:
        explicit NotAtomType(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class UnexpectedType : public RuntimeError {
    public:
        explicit UnexpectedType(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class UnsupportedSyntax : public RuntimeError {
    public:
        explicit UnsupportedSyntax(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class MissBracket : public RuntimeError {
    public:
        explicit MissBracket(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };
}

#endif //GI_EXCEPTION_H
