#include <iostream>
#include <parser.h>
#include <exception.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace lexers;
using namespace parser;
using namespace exception;

int main() {
    try {
        el::Logger *parserLogger = el::Loggers::getLogger("parser");
        el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                                           "[%logger] %msg [%fbase:%line]");
        Scope s;
        lexers::Lexer lex;
        lex.appendExp("(load \"setup.scm\")").appendExp("(load \"benchmark.scm\")");
        lex.appendExp("#(length (l-painter default))");
        lex.appendExp("(length ((beside (below painter (flip-vert painter)) (below painter (flip-vert painter))) default))");
        auto res = parseAllExpr(lex)->eval(s);
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        if (numPtr) {
            cout << numPtr->getValue() << endl;
        } else {
            cout << "ERROR" << endl;
        }
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
    return 0;
}
