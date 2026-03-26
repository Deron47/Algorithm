#include "pch.h"
#include "CppUnitTest.h"
#include "..\Practice 2 Friday 13 march/Integral.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Lab2Tests
{
    TEST_CLASS(SimpsonTests)
    {
    public:

        TEST_METHOD(TestWithN2)
        {
            double expected = 115.333333;
            double actual = calculateSimpson(5.0, 6.0, 2);
            Assert::AreEqual(expected, actual, 0.0001, L"Ошибка при N=2");
        }

        TEST_METHOD(TestWithN10)
        {
            double expected = 115.333333;
            double actual = calculateSimpson(5.0, 6.0, 10);
            Assert::AreEqual(expected, actual, 0.0001, L"Ошибка при N=10");
        }

        TEST_METHOD(TestWithN100)
        {
            double expected = 115.333333;
            double actual = calculateSimpson(5.0, 6.0, 100);
            Assert::AreEqual(expected, actual, 0.0001, L"Ошибка при N=100");
        }
    };
}