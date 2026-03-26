#include <iostream>
#include <iomanip>
#include <cmath>
#include "Integral.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    double a = 5.0; 
    double b = 6.0; 
    double exactValue = 115.333333333333; // Точное значение, которое посчитали

    int n_values[] = { 2, 10, 50, 100, 1000, 10000 };

    cout << "Точное аналитическое значение: " << exactValue << endl;
    cout << "N (разбиений) / Вычисленная площадь / Разница (ошибка)" << endl;

    for (int i = 0; i < 6; i++) {
        int n = n_values[i];

        double result = calculateSimpson(a, b, n);

        double error = abs(exactValue - result);

        cout << setw(14) << n << " | "
            << fixed << setprecision(6) << setw(19) << result << " | "
            << fixed << setprecision(10) << error << endl;
    }
    return 0;
}