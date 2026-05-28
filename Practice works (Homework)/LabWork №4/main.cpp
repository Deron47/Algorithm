#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

const int N = 1000;
// Обычная быстрая сортировка
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot_idx = partition(arr, low, high);
        quicksort(arr, low, pivot_idx - 1);
        quicksort(arr, pivot_idx + 1, high);
    }
}

// Параллельная быстрая сортировка
void parallel_quicksort(vector<int>& arr, int low, int high, int num_threads) {
    if (low >= high) return;

    if (num_threads <= 1 || (high - low) < N) {
        quicksort(arr, low, high);
        return;
    }

    int pivot_idx = partition(arr, low, high);
    thread left_thread([&arr, low, pivot_idx, num_threads]() {
        parallel_quicksort(arr, low, pivot_idx - 1, num_threads / 2);
        });

    parallel_quicksort(arr, pivot_idx + 1, high, num_threads - num_threads / 2);
    left_thread.join();
}

// Функции замера времени

double measure_qs(vector<int> arr) {
    auto start = chrono::high_resolution_clock::now();
    quicksort(arr, 0, (int)arr.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count();
}

double measure_pqs(vector<int> arr, int threads) {
    auto start = chrono::high_resolution_clock::now();
    parallel_quicksort(arr, 0, (int)arr.size() - 1, threads);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count();
}

// Главная функция

int main() {
    setlocale(LC_ALL, "Russian");

    vector<int> sizes = { 100, 1000, 10000, 20000, 30000, 40000, 50000 };

    mt19937 rng(42);
    uniform_int_distribution<int> dist(1, 1000000);

    cout << fixed << setprecision(7);
    cout << left
        << setw(10) << "Размер"
        << setw(14) << "БС (сек)"
        << setw(14) << "БС_П 2 пот"
        << setw(14) << "БС_П 4 пот"
        << setw(14) << "БС_П 8 пот"
        << "\n";

    for (int size : sizes) {
        vector<int> arr(size);
        for (int& x : arr) x = dist(rng);

        double t1 = measure_qs(arr);
        double t2 = measure_pqs(arr, 2);
        double t4 = measure_pqs(arr, 4);
        double t8 = measure_pqs(arr, 8);

        cout << left
            << setw(10) << size
            << setw(14) << t1
            << setw(14) << t2
            << setw(14) << t4
            << setw(14) << t8
            << "\n";
    }

    return 0;
}