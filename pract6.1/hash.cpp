#include <iostream>
#include <vector>
#include <string>
#include <locale>

using namespace std;

struct Data {
    long long int phoneNumber;
    string ownerAddress;
};

class HashTable {
private:
    vector<Data*> table;
    int size;
    int count;

    int hash(long long int key, int prime = 31) const {
        return (key % prime) % size;
    }

    int hash2(long long int key, int prime = 17) const {
        return (key % prime) % size;
    }

    void rehash() {
        cout << "\nРасширение размера таблицы и рехеширование.\n";
        vector<Data*> old_table = table;
        size *= 2;
        table.clear();
        table.resize(size, nullptr);
        count = 0;
        for (auto element : old_table) {
            if (element != nullptr) {
                insert(element->phoneNumber, element->ownerAddress, false);
            }
            delete element;
        }
    }

public:
    HashTable(int initial_size = 8) : size(initial_size), count(0) {
        table.resize(size, nullptr);
    }

    ~HashTable() {
        for (auto element : table) {
            delete element;
        }
    }

    void insert(long long int phoneNumber, const string& ownerAddress, bool message_flag=true) {
        if (count > size / 2) {
            rehash();
        }

        int index = hash(phoneNumber);
        int step = hash2(phoneNumber);
        while (table[index] != nullptr) {
            index = (index + step) % size;
        }

        table[index] = new Data{ phoneNumber, ownerAddress };
        count++;
        if (message_flag) {
            cout << "\nЭлемент с номером телефона " << phoneNumber << " добавлен.\n";
        }
    }

    void search(long long int phoneNumber) const {
        int index = hash(phoneNumber);
        int step = hash2(phoneNumber);
        for (int i = 0; i < size; i++) {
            if (table[index] != nullptr && table[index]->phoneNumber == phoneNumber) {
                cout << "\nНайден элемент: номер телефона = " << phoneNumber << ", адрес владельца = " << table[index]->ownerAddress << "\n";
                return;
            }
            index = (index + step) % size;
        }
        cout << "\nНомер телефона " << phoneNumber << " не найден.\n";
    }

    void remove(long long int phoneNumber) {
        int index = hash(phoneNumber);
        int step = hash2(phoneNumber);
        for (int i = 0; i < size; i++) {
            if (table[index] != nullptr && table[index]->phoneNumber == phoneNumber) {
                delete table[index];
                table[index] = nullptr;
                count--;
                cout << "\nЭлемент с номером телефона " << phoneNumber << " удален.\n";
                return;
            }
            index = (index + step) % size;
        }
        cout << "\nОшибка: номер телефона " << phoneNumber << " не найден.\n";
    }

    void display() const {
        cout << "\nСодержимое хеш-таблицы:" << endl;
        for (int i = 0; i < size; ++i) {
            if (table[i] != nullptr) {
                cout << "\nЯчейка " << i << ": номер телефона = " << table[i]->phoneNumber << ", адрес владельца = " << table[i]->ownerAddress << endl;
            }
            else {
                cout << "\nЯчейка " << i << ": пусто" << endl;
            }
        }
        cout << endl;
    }
};

void userInterface(HashTable& hashTable) {
    int command;
    while (true) {
        cout << "\nДоступыне команды:" << "\n 1. Добавить элемент"
            << "\n 2. Удалить элемент" << "\n 3. Найти элемент"
            << "\n 4. Отобразить таблицу" << "\n 5. Выйти\n";
        cout << "\nВведите номер команды: ";
        cin >> command;
        switch (command)
        {
        case 1: {
            long long int phoneNumber;
            string ownerAddress;
            cout << "\nВведите номер телефона: ";
            cin >> phoneNumber;
            cout << "\nВведите адрес владельца: ";
            cin >> ownerAddress;
            hashTable.insert(phoneNumber, ownerAddress);
            break;
        }
        case 2: {
            long long int phoneNumber;
            cout << "\nВведите номер телефон для удаления: ";
            cin >> phoneNumber;
            hashTable.remove(phoneNumber);
            break;
        }
        case 3: {
            long long int phoneNumber;
            cout << "\nВведите номер телефона для поиска: ";
            cin >> phoneNumber;
            hashTable.search(phoneNumber);
            break;
        }
        case 4: {
            hashTable.display();
            break;
        }
        case 5: {
            return;
        }
        default: {
            cout << "\nНеизвестная команда. Попробуйте снова.\n";
        }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    HashTable hashTable;
    hashTable.insert(9154318262, "ул. Шолохова, дом 5, кв 43");
    hashTable.insert(9164587270, "ул. Лукинская, дом 1, кв 321");
    hashTable.insert(9708762340, "ул. Скульптора Мухиной, дом 17 строение 2, кв 52");
    hashTable.insert(9126542780, "ул. Боровское шоссе, дом 40 корпус 7, кв 8");
    hashTable.insert(9189541234, "ул. Федосино, дом 3, кв 97");
    userInterface(hashTable);

    return 0;
}
