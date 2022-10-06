#include "frustum.h"
#include "constant_test.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Quaternion.
class FrustumTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  int fails;

  FrustumTest() {
     // You can do set-up work for each test here.
  }

  ~FrustumTest() override {
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

//TEST_F(Frustum, MethodDefaultConstructor) {
//  //EXPECT_EQ(v.x, 0);
//
//  EXPECT_TRUE(Color::NAMED_COLORS.at("indianred")==0xCD5C5C);
//}

TEST_F(FrustumTest, MethodSetFromProjectionMatrix) {
    //Frustum c(200,150,124);
    Matrix4 m;
    m.set(5,3,0,0,2,4,0,0,0,0,1,0,0,0,0,1);

    Frustum f;
    f.setFromProjectionMatrix(m);

    //EXPECT_TRUE(f.planes[0].constant==8);
    EXPECT_FLOAT_EQ(f.planes[0].normal.x,-0.85749292571254409);
    EXPECT_EQ(f.planes[0].normal.y,1);
    EXPECT_EQ(f.planes[0].normal.z,1);
    EXPECT_EQ(f.planes[0].constant,1);
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}