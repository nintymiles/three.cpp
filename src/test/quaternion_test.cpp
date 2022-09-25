#include "quaternion.h"
#include "constant_test.h"

#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {

// The fixture for testing class Quaternion.
class QuaternionTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  Quaternion q;

  QuaternionTest() {
     // You can do set-up work for each test here.
  }

  ~QuaternionTest() override {
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

// Tests that the Quaternion::Quaternion() method.
TEST_F(QuaternionTest, MethodDefaultConstructor) {
  //EXPECT_EQ(v.x, 0);
  q = Quaternion();
  EXPECT_TRUE(q.x()==0 && q.y()==0 && q.z()==0 && q.w()==1);
}

// Tests that Quaternion does set.
TEST_F(QuaternionTest, MethodSet) {
  // Exercises the xyz feature of Quaternion.
  q.set(x,y,z,w);
  EXPECT_TRUE(q.x()==x && q.y()==y && q.z()==z && q.w()==w);
}

TEST_F(QuaternionTest, MethodDot) {
  Quaternion a{0,0,0,1};
	Quaternion b{0,0,0,1};

  std::cout << "a.dot(b)= " << a.dot(b) << std::endl;
  EXPECT_EQ(a.dot(b),1);

			// a = new Quaternion( 1, 2, 3, 1 );
			// b = new Quaternion( 3, 2, 1, 1 );

			// assert.ok( a.dot( b ) === 11, 'Passed!' );
}


}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}