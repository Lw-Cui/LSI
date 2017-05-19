#ifndef GI_TEST_MACRO_H
#define GI_TEST_MACRO_H

#define CREATE_CONTEXT() \
    auto s = std::make_shared<Scope>();\
    lexers::Lexer lex;\
    pExpr ast, res;\
    std::shared_ptr<NumberAST> numPtr;\
    std::shared_ptr<BooleansTrueAST> trueBool;\
    std::shared_ptr<BooleansFalseAST> falseBool;

#define TO_NUM_PTR(res)\
    std::dynamic_pointer_cast<NumberAST>(res)

#define TO_TRUE_PTR(res)\
    std::dynamic_pointer_cast<BooleansTrueAST>(res)

#define TO_FALSE_PTR(res)\
    std::dynamic_pointer_cast<BooleansFalseAST>(res)

#define REPL_COND(str, condition)\
    lex.appendExp(str);\
    ast = parseAllExpr(lex);\
    res = ast->eval(s, ast);\
    ASSERT_TRUE(condition);\
    numPtr = TO_NUM_PTR(res);\
    trueBool = TO_TRUE_PTR(res);\
    falseBool = TO_FALSE_PTR(res);

#define BEG_TRY\
    try {

#define END_TRY\
    } catch (RuntimeError &e) {\
        CLOG(DEBUG, "exception") << e.what();\
        throw;\
    }

#endif
