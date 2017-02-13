#include <exception>
#include <iostream>
#include <vector>
#include <context.h>
#include <parser.h>
#include <image.h>
#include <exception.h>
#include <cxxopts.hpp>
#include <CLIbuiltinDrawAST.h>

using namespace std;
using namespace ast;
using namespace parser;
using namespace lexers;
using namespace cxxopts;
using namespace context;
using namespace exception;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger *parserLogger = el::Loggers::getLogger("parser");
    el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger] %msg [%fbase:%line]");
    try {
        Options options(argv[0], " - command line options");
        options.add_options()("o,output", "output filename", value<std::string>()->default_value("output.bmp"));
        options.parse(argc, argv);

        Scope scope;
        Image image(1000, 1000);
        scope.addBuiltinFunc("#painter", std::make_shared<ast::CLIBuiltinDrawAST>(image));
        Lexer lex;
        lex.appendExp("(load \"setup.scm\")").appendExp(string("(load \"") + argv[argc - 1] + "\")");
        shared_ptr<ExprAST> ptr = parseAllExpr(lex)->eval(scope);
        if (ptr) {
            cout << ptr->display();
        }
        image.save(options["output"].as<string>().c_str());
    } catch (RuntimeError &e) {
        cout << e.what() << endl;
        throw;
    } catch (const OptionException &e) {
        cout << "error parsing options: " << e.what()
             << "Try \' --help\' for more information." << endl;
        throw;
    } catch (std::exception &e) {
        cout << e.what() << endl;
        throw;
    }
    return 0;
}
