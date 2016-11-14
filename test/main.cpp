#include <gtest/gtest.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger* lexerLogger = el::Loggers::getLogger("lexer");
    el::Logger* parserLogger = el::Loggers::getLogger("parser");
    el::Logger* ASTLogger = el::Loggers::getLogger("AST");
    el::Logger* exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger][%func] %msg [%fbase:%line]");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
