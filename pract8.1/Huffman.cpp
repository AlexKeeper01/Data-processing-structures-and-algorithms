#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <Windows.h>
#include <algorithm>

using namespace std;

struct Symbol {
    char character;
    int frequency;
    double probability;
    string code;
};

vector<Symbol> calculateFrequencies(const string& input) {
    unordered_map<char, int> frequencies;
    int total_chars = input.size();
    for (char ch : input) {
        frequencies[ch]++;
    }

    vector<Symbol> symbols;
    for (const auto& pair : frequencies) {
        Symbol symbol;
        symbol.character = pair.first;
        symbol.frequency = pair.second;
        symbol.probability = static_cast<int>((static_cast<double>(pair.second) / total_chars) * 100) / 100.0;
        symbols.push_back(symbol);
    }

    return symbols;
}

void printTableOfFrequencies(vector<Symbol>& symbols) {
    cout << setw(10) << "Символ" << setw(10) << "Частота" << setw(15) << "Вероятность" << endl;
    cout << string(35, '-') << endl;

    sort(symbols.begin(), symbols.end(), [&](const Symbol& a, const Symbol& b) {return a.probability > b.probability;});

    for (const auto& symbol : symbols) {
        cout << setw(5) << "'" << symbol.character << "'"
            << setw(10) << symbol.frequency
            << setw(15) << symbol.probability << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string input = "попов алексей александрович";

    vector<Symbol> symbols = calculateFrequencies(input);

    printTableOfFrequencies(symbols);

    return 0;
}
