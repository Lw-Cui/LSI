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
        Options options(argv[0], " - Scheme Interpreter/painter command line options");
        options.add_options()("o,output", "output image", value<std::string>()->default_value("output.bmp"))
                ("src", "src filename", cxxopts::value<std::vector<std::string>>())
                ("p,path", "stdlib path", cxxopts::value<std::string>())
                ("nostdlib", "Do not use stdlib")
                ("nopainter", "Do not use painter-related lib")
                ("h,help", "Print help");
        options.parse_positional("src");
        options.parse(argc, argv);

        if (options.count("h")) {
            cout << options.help({""}) << endl;
            return 0;
        }
        if (!options.count("nostdlib") && !options.count("path")) {
            cout << "You must specify -nostdlib or -path" << endl;
            return 0;
        }
        Lexer lex;
        Scope scope;
        auto path = options["path"].as<std::string>();
        Image image(1000, 1000);
        if (!options.count("nostdlib")) {
            lex.appendExp("(load \"" + path + "/Base.scm\")");
        }
        if (!options.count("nopainter") && !options.count("nostdlib")) {
            scope.addBuiltinFunc("#painter", std::make_shared<ast::CLIBuiltinDrawAST>(image));
            lex.appendExp("(load \"" + path + "/Shape.scm\")");
            lex.appendExp("(load \"" + path + "/Frame.scm\")");
        }
        parseAllExpr(lex)->eval(scope);
        auto &v = options["src"].as<std::vector<std::string>>();
        for (const auto &s : v) {
            lex.appendExp(string("(load \"") + s + "\")");
            shared_ptr<ExprAST> ptr = parseAllExpr(lex)->eval(scope);
            if (ptr) {
                cout << ptr->display();
            }
        }
        if (!options.count("nopainter") && !options.count("nostdlib")) {
            image.save(options["output"].as<string>().c_str());
        }
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
