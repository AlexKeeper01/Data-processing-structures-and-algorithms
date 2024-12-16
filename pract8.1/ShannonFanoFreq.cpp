#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <Windows.h>
#include <cctype>

using namespace std;

struct Symbol {
    char character;
    int frequency;
    string code;
};

int splitSymbols(vector<Symbol>& symbols, int start, int end) {
    int total = 0;
    for (int i = start; i <= end; i++)
        total += symbols[i].frequency;

    int half = total / 2;
    int sum = 0;

    for (int i = start; i <= end; i++) {
        sum += symbols[i].frequency;
        if (sum >= half)
            return i;
    }
    return start;
}

void shannonFano(vector<Symbol>& symbols, int start, int end) {
    if (start >= end)
        return;

    int split = splitSymbols(symbols, start, end);

    for (int i = start; i <= split; i++)
        symbols[i].code += "0";
    for (int i = split + 1; i <= end; i++)
        symbols[i].code += "1";

    shannonFano(symbols, start, split);
    shannonFano(symbols, split + 1, end);
}

unordered_map<char, string> encodeShannonFano(const string& text, vector<Symbol>& symbols) {
    unordered_map<char, int> frequencyMap;
    for (char ch : text)
        frequencyMap[ch]++;

    for (const auto& pair : frequencyMap)
        symbols.push_back({ pair.first, pair.second, "" });

    sort(symbols.begin(), symbols.end(), [](const Symbol & a, const Symbol & b) {return a.frequency > b.frequency;});

    shannonFano(symbols, 0, symbols.size() - 1);

    unordered_map<char, string> codeMap;
    for (const Symbol& symbol : symbols)
        codeMap[symbol.character] = symbol.code;

    return codeMap;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string text = "Плыл по морю чемодан, В чемодане был диван, На диване ехал слон. Кто не верит – выйди вон!";
    vector<Symbol> symbols;
    unordered_map<char, string> codeMap = encodeShannonFano(text, symbols);

    cout << "Символы, их частота появления и коды:\n";
    for (const Symbol& symbol : symbols)
        cout << "'" << symbol.character << "'"
        << " -> Частота появления: " << symbol.frequency
        << ", Код: " << symbol.code << endl;

    cout << "\nЗакодированная строка: ";
    for (char ch : text)
        cout << codeMap[ch];
    cout << endl;

    return 0;
}
