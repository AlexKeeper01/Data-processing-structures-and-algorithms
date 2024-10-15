#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <bitset>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

int Program_1_a(int input) {
    unsigned char x = (char)input;
    unsigned char mask = 1;
    x = x & (~(mask << 4));
    return x;
}

int Program_1_b(int input) {
    unsigned char x = (char)input;
    unsigned char mask = 1;
    x = x & (~(mask << 6));
    x = x | (mask << 6);
    return x;
}

int Program_1_c(int input) {
    unsigned int x = input;
    const int n = sizeof(int) * 8;
    unsigned maska = (1 << (n - 1));
    cout << "Начальный вид маски: " << bitset<n>(maska) << endl;
    cout << "Результат: ";
    for (int i = 1; i <= n; i++) {
        cout << ((x & maska) >> (n - i));
        maska = maska >> 1;
    }
    cout << endl;
    system("pause");
    return 0;
}

void Program_2_a(unsigned char* arr, int size) {
    unsigned char bit_mask = 0;
    for (int i = 0; i < size; i++) {
        bit_mask |= (1 << arr[i]);
    }
    cout << "Отсортированные числа: ";
    for (int i = 0; i < 8; i++) {
        if (bit_mask & (1 << i)) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void Program_2_b(unsigned char* arr, int size) {
    unsigned long long bit_mask = 0;
    for (int i = 0; i < size; i++) {
        bit_mask |= (1ULL << arr[i]);
    }
    cout << "Отсортированные числа: ";
    for (int i = 0; i < 64; i++) {
        if (bit_mask & (1ULL << i)) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void Program_2_c(unsigned char* arr, int size) {
    unsigned char bit_mask[8] = { 0 };
    for (int i = 0; i < size; i++) {
        int index = arr[i] / 8;
        int bit_position = arr[i] % 8;
        bit_mask[index] |= (1 << bit_position);
    }
    cout << "Отсортированные числа: ";
    for (int i = 0; i < 64; i++) {
        int index = i / 8;
        int bit_position = i % 8;
        if (bit_mask[index] & (1 << bit_position)) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void Program_3_a(const string& input_file, const string& output_file, int max_value) {
    vector<unsigned char> bit_array((max_value / 8) + 1, 0); // битовый массив

    auto set_bit = [&bit_array](int num) {  // lambda-функция для установки 1 в битовом массиве
        int byte_index = num / 8;
        int bit_index = num % 8;
        bit_array[byte_index] |= (1 << bit_index);
        };

    ifstream infile(input_file); // чтение файла
    int number;
    while (infile >> number) {
        set_bit(number);
    }
    infile.close();

    ofstream outfile(output_file); // сортировка и запись
    for (int i = 0; i <= max_value; i++) {
        int byte_index = i / 8;
        int bit_index = i % 8;
        if (bit_array[byte_index] & (1 << bit_index)) {
            outfile << i << "\n";
        }
    }
    outfile.close();
}

int Program_3_b() {
    int max_value = 10000000 - 1;
    vector<unsigned char> bit_array((max_value / 8) + 1, 0);
    // size_t — это встроенный тип данных, который используется для представления размеров и количества объектов в памяти.
    size_t memory_size = bit_array.size() * sizeof(unsigned char);
    cout << "Объем оперативной памяти, занимаемый битовым массивом: " << memory_size / (1024 * 1024) << " Мбайт" << endl;
    return 0;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Program_3_b();
}

/*
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int max_value = 10000000 - 1;
    string input_file = "unique_numbers.txt";
    string output_file = "sorted_numbers.txt";
    auto start = chrono::high_resolution_clock::now();
    Program_3_a(input_file, output_file, max_value);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Файл отсортирован и записан в " << output_file << endl;
    cout << "Время выполнения: " << duration.count() << " секунд." << endl;
    return 0;
}
*/
