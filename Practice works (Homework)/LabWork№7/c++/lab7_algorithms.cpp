#include "lab7_algorithms.h"

using namespace std;


// Задача 4. Разные деревья поиска

long long countSearchTrees(int n) {
    vector<long long> dp(n + 1, 0);

    dp[0] = 1;

    if (n >= 1) {
        dp[1] = 1;
    }

    for (int nodesCount = 2; nodesCount <= n; nodesCount++) {
        long long total = 0;

        for (int leftCount = 0; leftCount < nodesCount; leftCount++) {
            int rightCount = nodesCount - 1 - leftCount;
            total += dp[leftCount] * dp[rightCount];
        }

        dp[nodesCount] = total;
    }

    return dp[n];
}


// Задача 7. Поле с цветочками

int maxFlowers(const vector<vector<int>>& field) {
    int n = field.size();
    int m = field[0].size();

    vector<vector<int>> dp(n, vector<int>(m, 0));

    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < m; j++) {
            int bestPrevious = 0;

            if (i + 1 < n) {
                bestPrevious = max(bestPrevious, dp[i + 1][j]);
            }

            if (j - 1 >= 0) {
                bestPrevious = max(bestPrevious, dp[i][j - 1]);
            }

            if (i == n - 1 && j == 0) {
                dp[i][j] = field[i][j];
            }
            else {
                dp[i][j] = bestPrevious + field[i][j];
            }
        }
    }

    return dp[0][m - 1];
}


// Задача 2. Первое и последнее вхождение числа

int findFirst(const vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int answer = -1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (nums[mid] == target) {
            answer = mid;

            right = mid - 1;
        }
        else if (nums[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return answer;
}


int findLast(const vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int answer = -1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (nums[mid] == target) {
            answer = mid;

            left = mid + 1;
        }
        else if (nums[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return answer;
}


// Задача 1. Расстановка ферзей


void backtrackQueens(
    int row,
    int n,
    vector<string>& board,
    vector<bool>& columns,
    vector<bool>& diag1,
    vector<bool>& diag2,
    vector<vector<string>>& solutions
) {
    if (row == n) {
        solutions.push_back(board);
        return;
    }

    for (int col = 0; col < n; col++) {
        int d1 = row + col;
        int d2 = row - col + n;

        if (columns[col] || diag1[d1] || diag2[d2]) {
            continue;
        }

        board[row][col] = 'Q';
        columns[col] = true;
        diag1[d1] = true;
        diag2[d2] = true;

        backtrackQueens(row + 1, n, board, columns, diag1, diag2, solutions);

        board[row][col] = '.';
        columns[col] = false;
        diag1[d1] = false;
        diag2[d2] = false;
    }
}


vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> solutions;
    vector<string> board(n, string(n, '.'));

    vector<bool> columns(n, false);
    vector<bool> diag1(2 * n, false);
    vector<bool> diag2(2 * n, false);

    backtrackQueens(0, n, board, columns, diag1, diag2, solutions);

    return solutions;
}