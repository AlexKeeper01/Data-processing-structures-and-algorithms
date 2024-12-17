#include <iostream>
#include <vector>
#include <cmath>
#include <Windows.h>

using namespace std;

bool isValid(const vector<int>& sequence) {
    for (size_t i = 1; i < sequence.size(); i++) {
        if (sequence[i] == 1 && sequence[i - 1] == 1) {
            return false;
        }
    }
    return true;
}

int bfCount(int n, int& bfOperations) {
    int total = 0;
    int combinations = pow(2, n);

    for (int i = 0; i < combinations; ++i) {
        vector<int> sequence;
        for (int j = 0; j < n; ++j) {
            sequence.push_back((i >> j) & 1);
        }
        bfOperations++;
        if (isValid(sequence)) {
            total++;
        }
    }
    return total;
}

int dpCount(int n, int& dpOperations) {
    if (n == 1) return 2;
    if (n == 2) return 3;

    int dp_0 = 1, dp_1 = 1;
    int new_dp_0, new_dp_1;

    for (int i = 2; i <= n; i++) {
        new_dp_0 = dp_0 + dp_1;
        new_dp_1 = dp_0;

        dp_0 = new_dp_0;
        dp_1 = new_dp_1;

        dpOperations += 2;
    }

    return dp_0 + dp_1;
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int n;
    cout << "Введите длину последовательности n: ";
    cin >> n;

    int bfOperations = 0;
    int dpOperations = 0;

    int resultDP = dpCount(n, dpOperations);
    cout << "Динамическое программирование:\n";
    cout << "Количество последовательностей: " << resultDP << endl;
    cout << "Количество операций: " << dpOperations << endl;

    int resultBF = bfCount(n, bfOperations);
    cout << "\nПолный перебор:\n";
    cout << "Количество последовательностей: " << resultBF << endl;
    cout << "Количество операций: " << bfOperations << endl;

    return 0;
}
