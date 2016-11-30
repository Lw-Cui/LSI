#include <gtest/gtest.h>
#include <utility.h>

TEST(TextTest, IndentationTest) {
    ASSERT_EQ(std::string{"Hello world"}, setIndentation(""));
}
