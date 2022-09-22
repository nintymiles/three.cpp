#include "vector3.h"
#include "constant_test.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Vector3.
class Vector3Test : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  Vector3 v;

  Vector3Test() {
     // You can do set-up work for each test here.
  }

  ~Vector3Test() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Vector3::Vector3() method.
TEST_F(Vector3Test, MethodDefaultConstructor) {
  //EXPECT_EQ(v.x, 0);
  v = Vector3();
  EXPECT_TRUE(v.x==0 && v.y==0 && v.z==0);
}

// Tests that Vector3 does set.
TEST_F(Vector3Test, MethodSet) {
  // Exercises the xyz feature of Vector3.
  v.set(x,y,z);
  EXPECT_TRUE(v.x==x && v.y==y && v.z==z);
}

TEST_F(Vector3Test, MethodSetComponent) {
  // Exercises the xyz feature of Vector3.
  v.setComponent(2,w);
  EXPECT_EQ(v.z,w);
}


}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}