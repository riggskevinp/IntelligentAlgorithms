#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../FuzzyControl.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(MembershipFunctions, Triangle) {
    auto t_mf = TriangleMF(20,60,80);
    EXPECT_EQ(t_mf(20),0);
    EXPECT_EQ(t_mf(60),1);
    EXPECT_EQ(t_mf(70),0.5);
}

TEST(MembershipFunctions, Inheritance) {
    MembershipFunction* t_mf = new TriangleMF(20,60,80);
    EXPECT_EQ((*t_mf)(20),0);
    EXPECT_EQ((*t_mf)(60),1);
    EXPECT_EQ((*t_mf)(70),0.5);
}

TEST(MembershipFunctions, Trapezoidal) {
    auto t_mf = TrapezoidalMF(20,40,70,90);
    EXPECT_EQ(t_mf(20),0);
    EXPECT_EQ(t_mf(50),1);
    EXPECT_EQ(t_mf(80),0.5);
}

TEST(MembershipFunctions, Gaussian) {
    auto t_mf = GaussianMF(50,20);
    EXPECT_EQ(t_mf(50),1);
    EXPECT_EQ(t_mf(40),t_mf(60));
    EXPECT_LT(t_mf(20),0.5);
}

TEST(MembershipFunctions, GeneralizedBell) {
    auto t_mf = GeneralizedBellMF(20, 10,50);
    EXPECT_EQ(t_mf(50),1);
    EXPECT_EQ(t_mf(30),t_mf(70));
    EXPECT_EQ(t_mf(30),0.5);
}

TEST(MembershipFunctions, Sigmoidal) {
    auto t_mf = SigmoidalMF(1, -5);
    EXPECT_EQ(t_mf(-5),0.5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}