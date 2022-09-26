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
  int fails;

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
     fails = 0;
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
  q = Quaternion();
  q.set(x,y,z,w);
  EXPECT_TRUE(q.x()==x && q.y()==y && q.z()==z && q.w()==w);
  EXPECT_EQ(q.x(),x);
  EXPECT_EQ(q.w(),w);
}

TEST_F(QuaternionTest, MethodDot) {
  Quaternion a{0,0,0,1};
  Quaternion b{0,0,0,1};

  std::cout << "a.dot(b)= " << a.dot(b) << std::endl;
  EXPECT_EQ(a.dot(b),1);

  fails += ::testing::Test::HasFailure();
}

TEST_F(QuaternionTest, MethodNormalizeLengthLengthSq) {
    q.set(x,y,z,w);

    EXPECT_NE(q.length(),1);
    EXPECT_NE(q.lengthSq(),1);

    q.normalize();
    EXPECT_EQ(q.length(),1);
    EXPECT_EQ(q.lengthSq(),1);

    q.set(0,0,0,0);
    EXPECT_EQ(q.length(),0);
    EXPECT_EQ(q.lengthSq(),0);

    q.normalize();
    EXPECT_EQ(q.lengthSq(),1);
    EXPECT_EQ(q.length(),1);

    fails += ::testing::Test::HasFailure();
}

TEST_F(QuaternionTest,MethodIdentity){
    q.set( x, y, z, w );
    q.identity();

    EXPECT_EQ(q.x(),0);
    EXPECT_EQ(q.y(),0);
    EXPECT_EQ(q.z(),0);
    EXPECT_EQ(q.w(),1);
}

TEST_F(QuaternionTest,MethodInvert){
    q.set( x, y, z, w );
    Quaternion a = q.clone().invert();

    EXPECT_EQ(q.x(),-a.x());
    EXPECT_EQ(q.y(),-a.y());
    EXPECT_EQ(q.z(),-a.z());
    EXPECT_EQ(q.w(),a.w());
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}