#pragma once

double targetFunction(double x) {
    return 4 * x * x - 6;
}

double calculateSimpson(double a, double b, int n) {
    if (n % 2 != 0) {
        n++;
    }

    double h = (b - a) / n; // Шаг разбиения
    double sum = targetFunction(a) + targetFunction(b); // Сумма крайних точек

    // Сумма нечетных узлов
    for (int i = 1; i < n; i += 2) {
        sum += 4 * targetFunction(a + i * h);
    }

    // Сумма четных узлов
    for (int i = 2; i < n - 1; i += 2) {
        sum += 2 * targetFunction(a + i * h);
    }

    return (h / 3.0) * sum;
}