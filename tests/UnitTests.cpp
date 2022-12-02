#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../FuzzyControl.hpp"

#include <limits>

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

TEST(FuzzySet, Sigmoidal) {
    auto t_mf = FuzzySet(new SigmoidalMF(1, -5));
    t_mf.area();
}

TEST(InferenceEngines, Simplified_PIE_with_CA){
    // Input Variable
    auto cold = FuzzySet(new TriangleMF(-0.0001,0,30));
    auto cool = FuzzySet(new TriangleMF(0,20,70));
    auto warm = FuzzySet(new TriangleMF(40,70,std::numeric_limits<double>::max()));

    // Output Variable
    auto fast = FuzzySet(new TriangleMF(70,80,90));
    auto medium = FuzzySet(new TriangleMF(50,60,70));
    auto slow = FuzzySet(new TriangleMF(10,20,30));

    auto ifTempIsColdThenFanIsFast = FuzzyRule({cold},fast);
    auto ifTempICoolThenFanIsMedium = FuzzyRule({cool},medium);
    auto ifTempIsWarmThenFanIsSlow = FuzzyRule({warm},slow);

    SimplifiedPIECA fc({ifTempIsColdThenFanIsFast,
                        ifTempICoolThenFanIsMedium,
                        ifTempIsWarmThenFanIsSlow,
    });

    EXPECT_DOUBLE_EQ(fc.fuzzDefuzz({0}), 80.0);
    EXPECT_DOUBLE_EQ(fc.fuzzDefuzz({35}), 60.0);
    EXPECT_DOUBLE_EQ(fc.fuzzDefuzz({90}), 20.0);

}

TEST(InferenceEngines, Simplified_MIE_with_CA_MultiIn){
    // Input Variable
    auto cold = FuzzySet(new TriangleMF(std::numeric_limits<int>::min(),0,30));
    auto cool = FuzzySet(new TriangleMF(-10,20,50));
    auto warm = FuzzySet(new TriangleMF(10,40,std::numeric_limits<int>::max()));

    // Input Variable
    auto dark = FuzzySet(new TriangleMF(std::numeric_limits<int>::min(),10,40));
    auto overcast = FuzzySet(new TriangleMF(10,40,70));
    auto sunny = FuzzySet(new TriangleMF(40,70,std::numeric_limits<int>::max()));

    // Output Variable
    auto fast = FuzzySet(new TriangleMF(70,80,90));
    auto medium = FuzzySet(new TriangleMF(50,60,70));
    auto slow = FuzzySet(new TriangleMF(10,20,30));

    auto rule1 = FuzzyRule({cold,dark},fast);
    auto rule2 = FuzzyRule({cool,overcast},medium);
    auto rule3 = FuzzyRule({warm,sunny},slow);

    SimplifiedMIECA fc({rule1,
                        rule2,
                        rule3,
                        });

    /*for(int i = -5; i < 100; i++){
        for(int j = -5; j < 100; j++){
            std::cout << fc.fuzzDefuzz({double(i),double(j)}) << std::endl;
        }
    }*/
}

TEST(InferenceEngines, InvertedPendulum){
    // Input Linguistic Variable "error"
    auto negativeLargeError = FuzzySet(new TriangleMF(std::numeric_limits<int>::min(), -M_PI/2, -M_PI/4));
    auto negativeSmallError = FuzzySet(new TriangleMF(-M_PI/2, -M_PI/4,0));
    auto zeroError = FuzzySet(new TriangleMF(-M_PI/4,0, M_PI/4));
    auto positiveSmallError = FuzzySet(new TriangleMF(0, M_PI/4, M_PI/2));
    auto positiveLargeError = FuzzySet(new TriangleMF(M_PI/4, M_PI/2, std::numeric_limits<int>::max()));

    // Input Linguistic Variable "change in error"
    auto negativeLargeErrorDelta = FuzzySet(new TriangleMF(std::numeric_limits<int>::min(), -M_PI/2, -M_PI/4));
    auto negativeSmallErrorDelta = FuzzySet(new TriangleMF(-M_PI/2, -M_PI/4,0));
    auto zeroErrorDelta = FuzzySet(new TriangleMF(-M_PI/4,0, M_PI/4));
    auto positiveSmallErrorDelta = FuzzySet(new TriangleMF(0, M_PI/4, M_PI/2));
    auto positiveLargeErrorDelta = FuzzySet(new TriangleMF(M_PI/4, M_PI/2, std::numeric_limits<int>::max()));


    // Output Linguistic Variable "control force"
    auto negativeLargeControl = FuzzySet(new TriangleMF(-30, -20, -10));
    auto negativeSmallControl = FuzzySet(new TriangleMF(-20, -10,0));
    auto zeroControl = FuzzySet(new TriangleMF(-10,0,10));
    auto positiveSmallControl = FuzzySet(new TriangleMF(0,10,20));
    auto positiveLargeControl = FuzzySet(new TriangleMF(10,20,30));


    auto rule1 = FuzzyRule({negativeLargeError,negativeLargeErrorDelta},positiveLargeControl);
    auto rule2 = FuzzyRule({negativeLargeError,negativeSmallError},positiveLargeControl);
    auto rule3 = FuzzyRule({negativeLargeError,zeroErrorDelta},positiveLargeControl);
    auto rule4 = FuzzyRule({negativeLargeError,positiveSmallErrorDelta},positiveSmallControl);
    auto rule5 = FuzzyRule({negativeLargeError,positiveLargeErrorDelta},zeroControl);

    auto rule6 = FuzzyRule({negativeSmallError,negativeLargeErrorDelta},positiveLargeControl);
    auto rule7 = FuzzyRule({negativeSmallError,negativeSmallError},positiveLargeControl);
    auto rule8 = FuzzyRule({negativeSmallError,zeroErrorDelta},positiveSmallControl);
    auto rule9 = FuzzyRule({negativeSmallError,positiveSmallErrorDelta},zeroControl);
    auto rule10 = FuzzyRule({negativeSmallError,positiveLargeErrorDelta},negativeSmallControl);

    auto rule11 = FuzzyRule({zeroError,negativeLargeErrorDelta},positiveLargeControl);
    auto rule12 = FuzzyRule({zeroError,negativeSmallError},positiveSmallControl);
    auto rule13 = FuzzyRule({zeroError,zeroErrorDelta},zeroControl);
    auto rule14 = FuzzyRule({zeroError,positiveSmallErrorDelta},negativeSmallControl);
    auto rule15 = FuzzyRule({zeroError,positiveLargeErrorDelta},negativeLargeControl);

    auto rule16 = FuzzyRule({positiveSmallError,negativeLargeErrorDelta},positiveSmallControl);
    auto rule17 = FuzzyRule({positiveSmallError,negativeSmallError},zeroControl);
    auto rule18 = FuzzyRule({positiveSmallError,zeroErrorDelta},negativeSmallControl);
    auto rule19 = FuzzyRule({positiveSmallError,positiveSmallErrorDelta},negativeLargeControl);
    auto rule20 = FuzzyRule({positiveSmallError,positiveLargeErrorDelta},negativeLargeControl);

    auto rule21 = FuzzyRule({positiveLargeError,negativeLargeErrorDelta},zeroControl);
    auto rule22 = FuzzyRule({positiveLargeError,negativeSmallError},positiveSmallControl);
    auto rule23 = FuzzyRule({positiveLargeError,zeroErrorDelta},negativeLargeControl);
    auto rule24 = FuzzyRule({positiveLargeError,positiveSmallErrorDelta},negativeLargeControl);
    auto rule25 = FuzzyRule({positiveLargeError,positiveLargeErrorDelta},negativeLargeControl);


    SumProdIECoA fc = SumProdIECoA({
        FuzzyRule({negativeLargeError,negativeLargeErrorDelta},positiveLargeControl),
        FuzzyRule({negativeLargeError,negativeSmallError},positiveLargeControl),
        FuzzyRule({negativeLargeError,zeroErrorDelta},positiveLargeControl),
        FuzzyRule({negativeLargeError,positiveSmallErrorDelta},positiveSmallControl),
        FuzzyRule({negativeLargeError,positiveLargeErrorDelta},zeroControl),

        FuzzyRule({negativeSmallError,negativeLargeErrorDelta},positiveLargeControl),
        FuzzyRule({negativeSmallError,negativeSmallError},positiveLargeControl),
        FuzzyRule({negativeSmallError,zeroErrorDelta},positiveSmallControl),
        FuzzyRule({negativeSmallError,positiveSmallErrorDelta},zeroControl),
        FuzzyRule({negativeSmallError,positiveLargeErrorDelta},negativeSmallControl),

        FuzzyRule({zeroError,negativeLargeErrorDelta},positiveLargeControl),
        FuzzyRule({zeroError,negativeSmallError},positiveSmallControl),
        FuzzyRule({zeroError,zeroErrorDelta},zeroControl),
        FuzzyRule({zeroError,positiveSmallErrorDelta},negativeSmallControl),
        FuzzyRule({zeroError,positiveLargeErrorDelta},negativeLargeControl),

        FuzzyRule({positiveSmallError,negativeLargeErrorDelta},positiveSmallControl),
        FuzzyRule({positiveSmallError,negativeSmallError},zeroControl),
        FuzzyRule({positiveSmallError,zeroErrorDelta},negativeSmallControl),
        FuzzyRule({positiveSmallError,positiveSmallErrorDelta},negativeLargeControl),
        FuzzyRule({positiveSmallError,positiveLargeErrorDelta},negativeLargeControl),

        FuzzyRule({positiveLargeError,negativeLargeErrorDelta},zeroControl),
        FuzzyRule({positiveLargeError,negativeSmallError},positiveSmallControl),
        FuzzyRule({positiveLargeError,zeroErrorDelta},negativeLargeControl),
        FuzzyRule({positiveLargeError,positiveSmallErrorDelta},negativeLargeControl),
        FuzzyRule({positiveLargeError,positiveLargeErrorDelta},negativeLargeControl)
    });

    for(int x = -2; x <= 2; x++){
        for(int y = -2; y <= 2; y++){
            std::cout << "x: " << x << " y: " << y << " ";
            std::cout << fc.fuzzDefuzz({double(x),double(y)}) << std::endl;
        }
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}