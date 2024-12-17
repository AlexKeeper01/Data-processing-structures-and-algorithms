#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>

using namespace std;

struct LZ78Pair {
    int index;
    char nextChar;

    LZ78Pair (int i, char next) : index(i), nextChar(next) {}
};

vector<LZ78Pair> encodeLZ78(const string& input) {
    unordered_map<string, int> dictionary;
    vector<LZ78Pair> compressedData;

    string current = "";
    int dictIndex = 1;

    for (char ch : input) {
        current += ch;

        if (dictionary.find(current) == dictionary.end()) {
            int prefixIndex = (current.size() > 1) ? dictionary[current.substr(0, current.size() - 1)] : 0;
            compressedData.emplace_back(prefixIndex, ch);

            dictionary[current] = dictIndex++;

            current = "";
        }
    }

    if (!current.empty()) {
        int prefixIndex = dictionary[current];
        compressedData.emplace_back(prefixIndex, '-');
    }

    return compressedData;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string input = "webwerbweberweberweb";

    vector<LZ78Pair> encodedData = encodeLZ78(input);

    cout << "Результат сжатия информации: ";
    for (const auto& pair : encodedData) {
        cout << "(" << pair.index << ", '" << pair.nextChar << "') ";
    }
    cout << endl;

    return 0;
}
