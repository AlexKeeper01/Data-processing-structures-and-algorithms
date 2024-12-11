#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

struct Insurance {
    int registration_number;
    char insurance_company[50];
};

struct IndexEntry {
    int registration_number;
    streampos offset;
};

bool compare(const IndexEntry& a, const IndexEntry& b) {
    return a.registration_number < b.registration_number;
}

vector<IndexEntry> createIndexTable(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        exit(1);
    }

    vector<IndexEntry> index_table;
    Insurance insurance;
    streampos offset = 0;

    while (file.read(reinterpret_cast<char*>(&insurance), sizeof(Insurance))) {
        index_table.push_back({ insurance.registration_number, offset });
        offset = file.tellg();
    }

    file.close();

    sort(index_table.begin(), index_table.end(), compare);

    return index_table;
}

streampos interpolationSearch(const vector<IndexEntry>& index_table, int key) {
    int low = 0, high = index_table.size() - 1;

    while (low <= high && key >= index_table[low].registration_number && key <= index_table[high].registration_number) {
        if (low == high) {
            if (index_table[low].registration_number == key)
                return index_table[low].offset;
            break;
        }

        if (key == index_table[high].registration_number) {
            return index_table[high].offset;
        }

        if (key == index_table[low].registration_number) {
            return index_table[low].offset;
        }

        int pos = low + ((key - index_table[low].registration_number) * (high - low)) /
            (index_table[high].registration_number - index_table[low].registration_number);

        if (pos < low) pos = low;
        if (pos > high) pos = high;

        if (index_table[pos].registration_number == key)
            return index_table[pos].offset;

        if (index_table[pos].registration_number < key) {
            low = pos + 1;
        }
        else {
            high = pos - 1;
        }
    }

    return -1;
}

Insurance readInsuranceByOffset(const string& filename, streampos offset) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        exit(1);
    }

    Insurance insurance;
    file.seekg(offset);
    file.read(reinterpret_cast<char*>(&insurance), sizeof(Insurance));
    file.close();

    return insurance;
}

int main() {
    setlocale(LC_ALL, "Russian");

    string filename = "insurance.bin";

    int search_key;
    cout << "Введите регистрационный номер для поиска: ";
    cin >> search_key;

    auto start_time = high_resolution_clock::now();

    vector<IndexEntry> index_table = createIndexTable(filename);

    streampos offset = interpolationSearch(index_table, search_key);

    auto end_time = high_resolution_clock::now();
    auto total_time = duration_cast<microseconds>(end_time - start_time).count();

    if (offset != -1) {
        Insurance insurance = readInsuranceByOffset(filename, offset);

        cout << "Запись найдена!" << endl;
        cout << "Регистрационный номер: " << insurance.registration_number << endl;
        cout << "Название страховой компании: " << insurance.insurance_company << endl;
    }
    else {
        cout << "Запись с регистрационным номером " << search_key << " не найдена." << endl;
    }

    cout << "Общее время выполнения: " << float(total_time) / 1000000.0 << " секунд" << endl;

    return 0;
}
