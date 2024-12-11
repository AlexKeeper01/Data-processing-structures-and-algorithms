#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

struct Insurance {
    int registration_number;
    char insurance_company[50];
};

int main() {
    setlocale(LC_ALL, "Russian");

    ifstream file("insurance.bin", ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка при открытии файла!" << endl;
        return 1;
    }

    int search_key;
    cout << "Введите регистрационный номер для поиска: ";
    cin >> search_key;

    Insurance insurance;
    bool found = false;

    auto start_time = high_resolution_clock::now();

    while (file.read(reinterpret_cast<char*>(&insurance), sizeof(Insurance))) {
        if (insurance.registration_number == search_key) {
            cout << "Запись найдена!" << endl;
            cout << "Регистрационный номер: " << insurance.registration_number << endl;
            cout << "Название страховой компании: " << insurance.insurance_company << endl;
            found = true;
            break;
        }
    }

    auto end_time = high_resolution_clock::now();
    auto elapsed_time = duration_cast<microseconds>(end_time - start_time).count();

    if (!found) {
        cout << "Запись с регистрационным номером " << search_key << " не найдена." << endl;
    }

    cout << "Время выполнения поиска: " << float(elapsed_time) / 1000000.0 << " секунд." << endl;

    file.close();
    return 0;
}
