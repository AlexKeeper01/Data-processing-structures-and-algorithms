#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

struct Insurance {
    int registration_number;
    char insurance_company[50];
};

int generateRandomNumber(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

string generateRandomCompany() {
    string companies[] = { "Росгосстрах", "Ингосстрах", "РЕСО-Гарантия", "АльфаСтрахование", "Согласие", "ВТБ Страхование" };
    int randomIndex = generateRandomNumber(0, 5);
    return companies[randomIndex];
}

void createFiles(const string& binary_filename, const string& text_filename, int record_count) {
    ofstream bin_file(binary_filename, ios::binary);
    ofstream txt_file(text_filename);

    if (!bin_file.is_open() || !txt_file.is_open()) {
        cout << "Ошибка при открытии файлов!" << endl;
        return;
    }

    vector<int> used_numbers;

    for (int i = 0; i < record_count; ++i) {
        Insurance insurance;
        int temp_number;

        do {
            temp_number = generateRandomNumber(100000, 999999);
        } while (find(used_numbers.begin(), used_numbers.end(), temp_number) != used_numbers.end());
        used_numbers.push_back(temp_number);
        insurance.registration_number = temp_number;
        string company = generateRandomCompany();
        strncpy_s(insurance.insurance_company, company.c_str(), sizeof(insurance.insurance_company));
        insurance.insurance_company[sizeof(insurance.insurance_company) - 1] = '\0';

        bin_file.write(reinterpret_cast<char*>(&insurance), sizeof(Insurance));

        txt_file << insurance.registration_number << " " << insurance.insurance_company << endl;
    }

    bin_file.close();
    txt_file.close();
}

int main() {
    string binary_filename = "insurance.bin";
    string text_filename = "insurance.txt";
    int record_count = 10000;

    createFiles(binary_filename, text_filename, record_count);
    return 0;
}
