#include <iostream>
#include <vector>
#include <string>

#include "lab7_algorithms.h"

using namespace std;


// Запуск задачи 4. Разные деревья поиска

void taskTrees() {
    int n;

    cout << "Введите n: ";
    cin >> n;

    cout << "Количество разных деревьев поиска: ";
    cout << countSearchTrees(n) << endl;
}


// Запуск задачи 7. Поле с цветочками

void taskFlowers() {
    int n, m;

    cout << "Введите n и m: ";
    cin >> n >> m;

    vector<vector<int>> field(n, vector<int>(m, 0));

    cout << "Введите поле из 0 и 1:" << endl;

    for (int i = 0; i < n; i++) {
        string row;
        cin >> row;

        for (int j = 0; j < m; j++) {
            field[i][j] = row[j] - '0';
        }
    }

    cout << "Максимальное количество цветочков: ";
    cout << maxFlowers(field) << endl;
}


// Запуск задачи 2. Первое и последнее вхождение числа

void taskBinarySearch() {
    int n;

    cout << "Введите размер массива: ";
    cin >> n;

    vector<int> nums(n);

    cout << "Введите отсортированный массив: ";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }

    int target;

    cout << "Введите искомое число: ";
    cin >> target;

    int first = findFirst(nums, target);
    int last = findLast(nums, target);

    if (first == -1) {
        cout << "Element not found in the array" << endl;
    }
    else {
        cout << "The first occurrence of element " << target;
        cout << " is located at index " << first << endl;

        cout << "The last occurrence of element " << target;
        cout << " is located at index " << last << endl;
    }
}


// Запуск задачи 1. Расстановка ферзей

void taskQueens() {
    int n;

    cout << "Введите N: ";
    cin >> n;

    vector<vector<string>> solutions = solveNQueens(n);

    if (solutions.empty()) {
        cout << "Решений нет" << endl;
        return;
    }

    cout << "Количество решений: " << solutions.size() << endl;
    cout << "Первая найденная расстановка:" << endl;

    for (int i = 0; i < n; i++) {
        cout << solutions[0][i] << endl;
    }
}

// Главное меню

int main() {
    setlocale(LC_ALL, "Russian");
    int choice = -1;

    while (choice != 0) {
        cout << endl;
        cout << "Лабораторная работа №7, вариант 8" << endl;
        cout << "1 - Разные деревья поиска" << endl;
        cout << "2 - Поле с цветочками" << endl;
        cout << "3 - Первое и последнее вхождение числа" << endl;
        cout << "4 - Расстановка ферзей" << endl;
        cout << "0 - Выход" << endl;
        cout << "Выберите задачу: ";

        cin >> choice;

        if (choice == 1) {
            taskTrees();
        }
        else if (choice == 2) {
            taskFlowers();
        }
        else if (choice == 3) {
            taskBinarySearch();
        }
        else if (choice == 4) {
            taskQueens();
        }
        else if (choice == 0) {
            cout << "Выход" << endl;
        }
        else {
            cout << "Неверный выбор" << endl;
        }
    }

    return 0;
}