#include <GUIbuiltinDrawAST.h>

using namespace std;

std::string ast::CLIBuiltinDrawAST::display() const {
    return "Finish Drawing";
}

ast::CLIBuiltinDrawAST::CLIBuiltinDrawAST(cimg_library::CImg<float> &im) : image{im} {
}

std::pair<float, float> ast::CLIBuiltinDrawAST::toPair(const std::shared_ptr<ExprAST> &ptr) {
    if (auto pairPtr = dynamic_pointer_cast<PairAST>(ptr)) {
        float first = (float) dynamic_pointer_cast<NumberAST>(pairPtr->data.first)->getValue();
        float second = (float) dynamic_pointer_cast<NumberAST>(pairPtr->data.second)->getValue();

        LOG(DEBUG) << "Parse pair: (" << first << ", " << 1000 - second << ")";
        return make_pair(first, 1000 - second);
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Cannot convert to pair");
    }
}

std::shared_ptr<ast::ExprAST>
ast::CLIBuiltinDrawAST::apply(const std::vector<std::shared_ptr<ast::ExprAST>> &actualArgs, context::Scope &s) {
    auto exprPtr = actualArgs.front()->eval(s);
    while (!dynamic_pointer_cast<NilAST>(exprPtr)) {
        auto pairPtr = dynamic_pointer_cast<PairAST>(exprPtr);
        auto pair = toPair(pairPtr->data.first);
        image(pair.first, pair.second, 0) = image(pair.first, pair.second, 1) = image(pair.first, pair.second, 2) = 0;
        exprPtr = pairPtr->data.second;
    }
    return std::make_shared<CLIBuiltinDrawAST>(image);
}
