#ifndef GI_EXCEPTION_H
#define GI_EXCEPTION_H

#include <string>
#include <vector>
#include <exception>

namespace exception {
    class RuntimeError : public std::runtime_error {
    public:
        explicit RuntimeError(const std::string &what) : runtime_error(what) {}
    };

    class NotPair : public RuntimeError {
    public:
        explicit NotPair(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class NotAtomType : public RuntimeError {
    public:
        explicit NotAtomType(const std::string &what_arg) : RuntimeError(what_arg) {
        }
    };

    class NotNumber : public NotAtomType {
    public:
        explicit NotNumber(const std::string &what_arg) : NotAtomType(what_arg) {
        }
    };

    class UnboundIdentifier : public RuntimeError {
    public:
        explicit UnboundIdentifier(const std::string &what_arg) : RuntimeError(what_arg) {
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
