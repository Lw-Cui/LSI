#include <sstream>
#include "visitor.h"

using namespace visitor;
using namespace std;

void DisplayVisitor::visitBooleansFalseAST(const ast::BooleansFalseAST &) {
    prettyPrint = "#f";
}

void DisplayVisitor::visitBooleansTrueAST(const ast::BooleansTrueAST &) {
    prettyPrint = "#t";
}

void DisplayVisitor::visitPairAST(const ast::PairAST &pair) {
    string newPrint;
    pair.data.first->accept(*this);
    newPrint += "(" + prettyPrint + ", ";
    pair.data.second->accept(*this);
    newPrint += prettyPrint + ")";
    prettyPrint = std::move(newPrint);
}

void DisplayVisitor::visitNilAST(const ast::NilAST &) {
    prettyPrint = "\'()";
}

void DisplayVisitor::visitLambdaAST(const ast::LambdaAST &) {
    prettyPrint = "#proceduce";
}

void DisplayVisitor::visitNumberAST(const ast::NumberAST &num) {
    ostringstream sout;
    sout << num.getValue();
    prettyPrint = sout.str();
}

std::string DisplayVisitor::to_string() const {
    return prettyPrint;
}

void DisplayVisitor::clear() {
    prettyPrint.clear();
}
