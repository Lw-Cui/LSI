#include <iostream>
#include <vector>
#include <context.h>
#include <parser.h>
#include <lexers.h>
#include <AST.h>
#include <GUIbuiltinDrawAST.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace ast;
using namespace parser;
using namespace context;
using namespace cimg_library;

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger *parserLogger = el::Loggers::getLogger("parser");
    el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger] %msg [%fbase:%line]");

    Scope scope;
    CImg<float> image(1000, 1000, 1, 3, 255);
    scope.addBuiltinFunc("#painter", std::make_shared<ast::CLIBuiltinDrawAST>(image));
    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")");
    //lex.appendExp("(#painter (line (cons 0 0) (cons 100 100)))");
    parser::parseAllExpr(lex)->eval(scope);
    image.save("pic.bmp");
    return 0;
}
