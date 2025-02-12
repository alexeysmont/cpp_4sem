#include <iostream>
#include <math.h>
#include <complex>
#include <vector>
#define C0 complex<double>(0)

using namespace std;


struct solution { //структура для хранения решений уравнения
    vector<complex<double>> x;
    bool inf; //флаг бесконечного числа решений
};


complex<double> discriminant(complex<double> a, complex<double> b, complex<double> c) {
    return b * b - complex< double >(4) * a * c;
}


solution* solve(complex<double> a, complex<double> b, complex<double> c) {
    solution* answer = new solution();
    complex< double > const k(2);

    answer->inf = (a == C0) && (b == C0) && (c == C0);
    if (answer->inf)
        return answer;

    if (a == C0 && b == C0)
        return answer;

    if (a == C0) {
        answer->x.push_back(-c / b);
        return answer;
    }

    if (discriminant(a, b, c) == complex<double>(0)) {
        answer->x.push_back(-b / (k * a) + sqrt(discriminant(a, b, c)) / (k * a));
    }
    else {
        answer->x.push_back(-b / (k * a) + sqrt(discriminant(a, b, c)) / (k * a));
        answer->x.push_back(-b / (k * a) - sqrt(discriminant(a, b, c)) / (k * a));
    }
    return answer;
}

//перегрузка оператора вывода для комплексных чисел
ostream& operator<< (ostream& os, complex<double> c) {
    os << (c.real() != 0 ? c.real() : fabs(c.real()));
    if (c.imag())
        os << ((c.imag() > 0) ? " + " : " - ") << fabs(c.imag()) << "i";
    return os;
}


int main(int argc, const char* argv[]) {
    cout << "enter the coefficients of the quadratic equation separated by a space" << endl;
    complex<double> a, b, c;
    cin >> a >> b >> c;

    solution* answer = solve(a, b, c);

    if (answer->inf) {
        cout << "the equation has infinity of solutions" << endl;
        return 0;
    }

    if (answer->x.size() == 0)
        cout << "the equation has no solutions" << endl;

    for (complex<double> i : answer->x) {
        cout << i << endl;
    }

    return 0;
}
