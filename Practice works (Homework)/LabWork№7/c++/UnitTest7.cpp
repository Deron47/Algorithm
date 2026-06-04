#include "pch.h"
#include "CppUnitTest.h"

#include "../LabWork№7/lab7_algorithms.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Lab7Variant8Tests
{
    TEST_CLASS(UnitTestLab7Variant8)
    {
    public:

        // Тесты для задачи 4. Разные деревья поиска

        TEST_METHOD(TestSearchTreesSmallValues)
        {
            Assert::AreEqual(1LL, countSearchTrees(0));
            Assert::AreEqual(1LL, countSearchTrees(1));
            Assert::AreEqual(2LL, countSearchTrees(2));
            Assert::AreEqual(5LL, countSearchTrees(3));
            Assert::AreEqual(14LL, countSearchTrees(4));
        }


        TEST_METHOD(TestSearchTreesMediumValues)
        {
            Assert::AreEqual(42LL, countSearchTrees(5));
            Assert::AreEqual(132LL, countSearchTrees(6));
        }


        // Тесты для задачи 7. Поле с цветочками

        TEST_METHOD(TestFlowersExample1)
        {
            vector<vector<int>> field = {
                {1, 0, 1},
                {1, 1, 0}
            };

            Assert::AreEqual(3, maxFlowers(field));
        }


        TEST_METHOD(TestFlowersExample2)
        {
            vector<vector<int>> field = {
                {1, 0, 0},
                {1, 1, 0},
                {0, 0, 1}
            };

            Assert::AreEqual(2, maxFlowers(field));
        }


        TEST_METHOD(TestFlowersOneCell)
        {
            vector<vector<int>> field = {
                {1}
            };

            Assert::AreEqual(1, maxFlowers(field));
        }


        TEST_METHOD(TestFlowersWithoutFlowers)
        {
            vector<vector<int>> field = {
                {0, 0, 0},
                {0, 0, 0},
                {0, 0, 0}
            };

            Assert::AreEqual(0, maxFlowers(field));
        }


        // Тесты для задачи 2. Первое и последнее вхождение

        TEST_METHOD(TestFindFirstAndLastExistingElement)
        {
            vector<int> nums = { 2, 5, 5, 5, 6, 6, 8, 9, 9, 9 };

            Assert::AreEqual(1, findFirst(nums, 5));
            Assert::AreEqual(3, findLast(nums, 5));
        }


        TEST_METHOD(TestFindFirstAndLastMissingElement)
        {
            vector<int> nums = { 2, 5, 5, 5, 6, 6, 8, 9, 9, 9 };

            Assert::AreEqual(-1, findFirst(nums, 4));
            Assert::AreEqual(-1, findLast(nums, 4));
        }


        TEST_METHOD(TestFindFirstAndLastAtArrayBeginning)
        {
            vector<int> nums = { 1, 1, 1, 2, 3, 4 };

            Assert::AreEqual(0, findFirst(nums, 1));
            Assert::AreEqual(2, findLast(nums, 1));
        }


        TEST_METHOD(TestFindFirstAndLastAtArrayEnd)
        {
            vector<int> nums = { 1, 2, 3, 4, 7, 7, 7 };

            Assert::AreEqual(4, findFirst(nums, 7));
            Assert::AreEqual(6, findLast(nums, 7));
        }


        // Тесты для задачи 1. Расстановка ферзей

        TEST_METHOD(TestQueensN1)
        {
            vector<vector<string>> solutions = solveNQueens(1);

            Assert::AreEqual(1, static_cast<int>(solutions.size()));
            Assert::AreEqual(string("Q"), solutions[0][0]);
        }


        TEST_METHOD(TestQueensN2)
        {
            vector<vector<string>> solutions = solveNQueens(2);

            Assert::AreEqual(0, static_cast<int>(solutions.size()));
        }


        TEST_METHOD(TestQueensN3)
        {
            vector<vector<string>> solutions = solveNQueens(3);

            Assert::AreEqual(0, static_cast<int>(solutions.size()));
        }


        TEST_METHOD(TestQueensN4)
        {
            vector<vector<string>> solutions = solveNQueens(4);

            Assert::AreEqual(2, static_cast<int>(solutions.size()));
        }


        TEST_METHOD(TestQueensN5)
        {
            vector<vector<string>> solutions = solveNQueens(5);

            Assert::AreEqual(10, static_cast<int>(solutions.size()));
        }
    };
}