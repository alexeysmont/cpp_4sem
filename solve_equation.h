#pragma once

#include <iostream>
#include <math.h>
#include <complex>
#include <vector>
#define C0 complex<double>(0)

using namespace std;

// Структура для хранения решений квадратного уравнения
struct solution {
    vector<complex<double>> x; // Вектор корней
    bool inf;                            // Флаг бесконечного числа решений
};

// Вычисление дискриминанта
complex<double> discriminant(complex<double> a, complex<double> b, complex<double> c);

// Решение квадратного уравнения
solution* solve(complex<double> a, complex<double> b, complex<double> c);

// Вывод результатов уравнения
void result(solution* answer);

