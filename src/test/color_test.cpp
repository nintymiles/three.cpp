#include "color.h"
#include "constant_test.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Quaternion.
class ColorTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  int fails;

  ColorTest() {
     // You can do set-up work for each test here.
  }

  ~ColorTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
     fails = 0;
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(ColorTest, MethodDefaultConstructor) {
  //EXPECT_EQ(v.x, 0);

  EXPECT_TRUE(Color::NAMED_COLORS.at("indianred")==0xCD5C5C);
}

TEST_F(ColorTest, MethodGetHexString) {
    Color c(200,150,124);
    std::string hexString = c.getHexString();

    EXPECT_TRUE(hexString=="0xCD5C5C");
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}