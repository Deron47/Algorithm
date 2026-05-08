#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>    
#include <chrono>    

#include "sorts.h"   

using namespace std;


void printArray(const vector<int>& arr) {
    int limit = (int)arr.size() < 20 ? (int)arr.size() : 20;
    for (int i = 0; i < limit; i++)
        cout << arr[i] << " ";
    if ((int)arr.size() > 20)
        cout << "... (eshhe " << arr.size() - 20 << " el.)";
    cout << endl;
}

vector<int> generateArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++)
        arr[i] = rand() % 10000 + 1;
    return arr;
}


template<typename Func>
double measureTime(Func f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end   = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    cout << "пример входных/выходных данных" << endl;

    vector<int> demo = { 64, 25, 12, 22, 11, 90, 3, 47 };
    cout << "Вход:      ";
    printArray(demo);

    vector<int> d1 = demo, d2 = demo, d3 = demo;

    mergeSort(d1, 0, (int)d1.size() - 1);
    cout << "MergeSort:  ";
    printArray(d1);

    radixSort(d2);
    cout << "RadixSort:  ";
    printArray(d2);

    quickSort(d3, 0, (int)d3.size() - 1);
    cout << "QuickSort:  ";
    printArray(d3);

    cout << endl;


    cout << "ВРЕМЯ" << endl;
    cout << "Size\t\tСлияние\t\tПоразрядная\t\tБыстрая" << endl;


    int sizes[] = { 1000, 5000, 10000, 50000, 100000 };

    for (int size : sizes) {

        vector<int> base = generateArray(size);
        vector<int> a1 = base, a2 = base, a3 = base, a4 = base;

        double t1 = measureTime([&]() { mergeSort(a1, 0, (int)a1.size() - 1); });
        double t2 = measureTime([&]() { radixSort(a2); });
        double t3 = measureTime([&]() { quickSort(a3, 0, (int)a3.size() - 1); });
        cout << size
             << "\t\t" << t1
             << "\t" << t2
             << "\t" << t3
             << endl;
    }

    cout << endl;
    cout << "Готово, можно закрывать это..." << endl;
    cin.get();

    return 0;
}
