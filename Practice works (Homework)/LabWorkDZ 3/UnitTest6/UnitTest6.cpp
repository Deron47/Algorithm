#include "pch.h"
#include "CppUnitTest.h"
#include "..\Practice 3 Homework\sorts.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SortsTests
{

    TEST_CLASS(MergeSortTests)
    {
    public:

        // Тест 1: обычный неотсортированный массив
        TEST_METHOD(TestMerge_RandomArray)
        {
            vector<int> arr = { 64, 25, 12, 22, 11 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: массив не отсортирован");
        }

        // Тест 2: уже отсортированный массив — не должен сломаться
        TEST_METHOD(TestMerge_AlreadySorted)
        {
            vector<int> arr = { 1, 2, 3, 4, 5 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: уже отсортированный сломался");
        }

        // Тест 3: массив отсортирован в обратном порядке
        TEST_METHOD(TestMerge_ReverseSorted)
        {
            vector<int> arr = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: обратный порядок не отсортирован");
        }

        // Тест 4: массив с одинаковыми элементами
        TEST_METHOD(TestMerge_AllEqual)
        {
            vector<int> arr = { 5, 5, 5, 5, 5 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: одинаковые элементы");
        }

        // Тест 5: один элемент
        TEST_METHOD(TestMerge_SingleElement)
        {
            vector<int> arr = { 42 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::AreEqual(42, arr[0], L"Ошибка: один элемент изменился");
        }

        // Тест 6: два элемента
        TEST_METHOD(TestMerge_TwoElements)
        {
            vector<int> arr = { 10, 3 };
            mergeSort(arr, 0, arr.size() - 1);
            Assert::AreEqual(3, arr[0], L"Ошибка: первый элемент неверный");
            Assert::AreEqual(10, arr[1], L"Ошибка: второй элемент неверный");
        }

        // Тест 7: проверка конкретных значений
        TEST_METHOD(TestMerge_KnownResult)
        {
            vector<int> arr = { 3, 1, 4, 1, 5, 9, 2, 6 };
            vector<int> sorted = { 1, 1, 2, 3, 4, 5, 6, 9 };
            mergeSort(arr, 0, arr.size() - 1);
            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(sorted[i], arr[i], L"Ошибка: неверное значение");
            }
        }
    };


    TEST_CLASS(RadixSortTests)
    {
    public:

        TEST_METHOD(TestRadix_RandomArray)
        {
            vector<int> arr = { 170, 45, 75, 90, 2, 802, 24, 66 };
            radixSort(arr);
            Assert::IsTrue(isSorted(arr), L"Ошибка: массив не отсортирован");
        }

        TEST_METHOD(TestRadix_AlreadySorted)
        {
            vector<int> arr = { 1, 2, 3, 100, 200 };
            radixSort(arr);
            Assert::IsTrue(isSorted(arr), L"Ошибка: уже отсортированный сломался");
        }

        TEST_METHOD(TestRadix_ReverseSorted)
        {
            vector<int> arr = { 500, 400, 300, 200, 100 };
            radixSort(arr);
            Assert::IsTrue(isSorted(arr), L"Ошибка: обратный порядок");
        }

        TEST_METHOD(TestRadix_AllEqual)
        {
            vector<int> arr = { 7, 7, 7, 7 };
            radixSort(arr);
            Assert::IsTrue(isSorted(arr), L"Ошибка: одинаковые элементы");
        }

        TEST_METHOD(TestRadix_SingleElement)
        {
            vector<int> arr = { 99 };
            radixSort(arr);
            Assert::AreEqual(99, arr[0], L"Ошибка: один элемент изменился");
        }

        TEST_METHOD(TestRadix_TwoElements)
        {
            vector<int> arr = { 100, 1 };
            radixSort(arr);
            Assert::AreEqual(1, arr[0], L"Ошибка: первый элемент неверный");
            Assert::AreEqual(100, arr[1], L"Ошибка: второй элемент неверный");
        }

        TEST_METHOD(TestRadix_KnownResult)
        {
            vector<int> arr = { 53, 89, 15, 45, 12 };
            vector<int> sorted = { 12, 15, 45, 53, 89 };
            radixSort(arr);
            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(sorted[i], arr[i], L"Ошибка: неверное значение");
            }
        }
    };


    TEST_CLASS(QuickSortTests)
    {
    public:

        TEST_METHOD(TestQuick_RandomArray)
        {
            vector<int> arr = { 33, 10, 59, 27, 1, 84 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: массив не отсортирован");
        }

        TEST_METHOD(TestQuick_AlreadySorted)
        {
            vector<int> arr = { 2, 4, 6, 8, 10 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: уже отсортированный сломался");
        }

        TEST_METHOD(TestQuick_ReverseSorted)
        {
            vector<int> arr = { 10, 9, 8, 7, 6, 5 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: обратный порядок");
        }

        TEST_METHOD(TestQuick_AllEqual)
        {
            vector<int> arr = { 3, 3, 3, 3, 3 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::IsTrue(isSorted(arr), L"Ошибка: одинаковые элементы");
        }

        TEST_METHOD(TestQuick_SingleElement)
        {
            vector<int> arr = { 7 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::AreEqual(7, arr[0], L"Ошибка: один элемент изменился");
        }

        TEST_METHOD(TestQuick_TwoElements)
        {
            vector<int> arr = { 50, 5 };
            quickSort(arr, 0, arr.size() - 1);
            Assert::AreEqual(5, arr[0], L"Ошибка: первый элемент неверный");
            Assert::AreEqual(50, arr[1], L"Ошибка: второй элемент неверный");
        }

        TEST_METHOD(TestQuick_KnownResult)
        {
            vector<int> arr = { 8, 3, 1, 5, 9, 2, 7 };
            vector<int> sorted = { 1, 2, 3, 5, 7, 8, 9 };
            quickSort(arr, 0, arr.size() - 1);
            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(sorted[i], arr[i], L"Ошибка: неверное значение");
            }
        }
    };
}
