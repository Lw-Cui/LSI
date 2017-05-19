#include <GUIbuiltinDrawAST.h>

using namespace std;

std::shared_ptr<ast::ExprAST>
ast::GUIBuiltinDrawAST::apply(const std::vector<pExpr> &actualArgs, pScope &s) {
    con::VertexArray vertex;
    auto exprPtr = actualArgs.front()->eval(s, actualArgs.front());
    while (!dynamic_pointer_cast<NilAST>(exprPtr)) {
        auto pairPtr = dynamic_pointer_cast<PairAST>(exprPtr);
        vertex.append(sf::Vertex{toVec2f(pairPtr->data.first), sf::Color::Black});
        exprPtr = pairPtr->data.second;
    }
    controller.appendShape(vertex);
    return std::make_shared<GUIBuiltinDrawAST>(controller);
}

sf::Vector2f ast::GUIBuiltinDrawAST::toVec2f(const shared_ptr<ast::ExprAST> &ptr) const {
    if (auto pairPtr = dynamic_pointer_cast<PairAST>(ptr)) {
        auto firstPtr = dynamic_pointer_cast<NumberAST>(pairPtr->data.first);
        auto secondPtr = dynamic_pointer_cast<NumberAST>(pairPtr->data.second);

        auto vec2f = sf::Vector2f(static_cast<float>(firstPtr->getValue()),
                                  static_cast<float>(secondPtr->getValue()));
        LOG(DEBUG) << "Parse vec2f: (" << vec2f.x << ", " << vec2f.y << ")";
        return vec2f;
    } else {
        CLOG(DEBUG, "exception");
        throw std::logic_error("Cannot convert to vector2f.");
    }
}

shared_ptr<ast::ExprAST> ast::GUIBuiltinDrawAST::toPairAST(const sf::Vector2f &vec) const {
    return make_shared<PairAST>(make_shared<NumberAST>(vec.x), make_shared<NumberAST>(vec.y));
}

ast::GUIBuiltinDrawAST::GUIBuiltinDrawAST(con::Controller &c) : controller{c} {}

void ast::GUIBuiltinDrawAST::accept(visitor::NodeVisitor &visitor) const {
}

