#include <GUIbuiltinFunc.h>

using namespace std;

std::shared_ptr<ast::ExprAST>
ast::BuiltinDrawAST::apply(const std::vector<std::shared_ptr<ast::ExprAST>> &actualArgs, context::Scope &s) {
    sf::VertexArray vertex;
    auto exprPtr = actualArgs.front()->eval(s);
    while (!dynamic_pointer_cast<NilAST>(exprPtr)) {
        auto pairPtr = dynamic_pointer_cast<PairAST>(exprPtr);
        vertex.append(sf::Vertex{toVec2f(pairPtr->data.first), sf::Color::Black});
        exprPtr = pairPtr->data.second;
    }
    controller.appendShape(vertex);
    return std::make_shared<BuiltinDrawAST>(controller);
}

sf::Vector2f ast::GUIBuiltinAST::toVec2f(const shared_ptr<ast::ExprAST> &ptr) const {
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

shared_ptr<ast::ExprAST> ast::GUIBuiltinAST::toPairAST(const sf::Vector2f &vec) const {
    return make_shared<PairAST>(make_shared<NumberAST>(vec.x), make_shared<NumberAST>(vec.y));
}

string ast::BuiltinDrawAST::display() const {
    return "Finished drawing.";
}

ast::BuiltinDrawAST::BuiltinDrawAST(con::Controller &c) : controller{c} {}

shared_ptr<ast::ExprAST>
ast::BuiltinLineAST::apply(const vector<shared_ptr<ast::ExprAST>> &actualArgs, context::Scope &s) {
    auto pointSet = getPointSet(toVec2f(actualArgs[0]->eval(s)), toVec2f(actualArgs[1]->eval(s)));
    std::shared_ptr<ExprAST> list = std::make_shared<NilAST>();

    for (auto vec: pointSet)
        list = std::make_shared<PairAST>(toPairAST(vec), list);

    return list;
}

std::vector<sf::Vector2f> ast::BuiltinLineAST::getPointSet(sf::Vector2f begin, sf::Vector2f end) const {
    auto deltaX = end.x - begin.x;
    auto deltaY = end.y - begin.y;

    std::function<sf::Vector2f(sf::Vector2f)> Op;
    if (deltaY >= 0 && deltaX >= 0)
        Op = [](sf::Vector2f v) -> sf::Vector2f {
            return v;
        };
    else if (deltaY > 0 && deltaX < 0)
        Op = [](sf::Vector2f v) -> sf::Vector2f {
            v.x = -v.x;
            return v;
        };
    else if (deltaX > 0 && deltaY < 0)
        Op = [](sf::Vector2f v) -> sf::Vector2f {
            v.y = -v.y;
            return v;
        };
    else
        Op = [](sf::Vector2f v) -> sf::Vector2f {
            v = -v;
            return v;
        };

    std::function<sf::Vector2f(sf::Vector2f)> reverse;
    if (abs(deltaX) > abs(deltaY)) {
        reverse = [](sf::Vector2f v) -> sf::Vector2f {
            return v;
        };
    } else {
        reverse = [](sf::Vector2f v) -> sf::Vector2f {
            std::swap(v.x, v.y);
            return v;
        };
    }

    return SimpleLinePointSet(reverse(Op(begin)), reverse(Op(end)));
}

std::vector<sf::Vector2f> ast::BuiltinLineAST::SimpleLinePointSet(sf::Vector2f begin, sf::Vector2f end) const {
    std::vector<sf::Vector2f> vec2;
    auto a = begin.y - end.y, b = end.x - begin.x;
    auto d = 2 * a + b;
    auto a2b2 = 2 * (a + b), a2 = a * 2;
    sf::Vector2f current{begin};
    while (current.x <= end.x) {
        if (d < 0) {
            current.y++;
            d += a2b2;
        } else {
            d += a2;
        }
        current.x++;
        vec2.push_back(current);
    }
    return std::move(vec2);
}
