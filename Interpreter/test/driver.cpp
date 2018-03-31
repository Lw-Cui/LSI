#include <gtest/gtest.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    START_EASYLOGGINGPP(argc, argv);
    el::Logger *parserLogger = el::Loggers::getLogger("parser");
    el::Logger *evaluatorLogger = el::Loggers::getLogger("evaluator");
    el::Logger *conextLogger = el::Loggers::getLogger("context");
    el::Logger *exceptionLogger = el::Loggers::getLogger("exception");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format,
                                       "[%logger] %msg [%fbase:%line]");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
