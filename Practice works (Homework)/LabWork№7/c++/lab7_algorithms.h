#pragma once

#include <vector>
#include <string>

using namespace std;

// Задача 4. Разные деревья поиска
long long countSearchTrees(int n);

// Задача 7. Поле с цветочками
int maxFlowers(const vector<vector<int>>& field);

// Задача 2. Первое и последнее вхождение числа
int findFirst(const vector<int>& nums, int target);
int findLast(const vector<int>& nums, int target);

// Задача 1. Расстановка ферзей
vector<vector<string>> solveNQueens(int n);