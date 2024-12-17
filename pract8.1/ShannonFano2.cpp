#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <Windows.h>
#include <cctype>
#include <bitset>

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
        if (sum >= half) {
            if (sum - half <= half - (sum - symbols[i].frequency)) {
                return i;
            }
            else {
                if (i - 1 >= start) {
                    return i - 1;
                }
                return i;
            }
        }
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

    sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {return a.frequency > b.frequency; });

    shannonFano(symbols, 0, symbols.size() - 1);

    unordered_map<char, string> codeMap;
    for (const Symbol& symbol : symbols)
        codeMap[symbol.character] = symbol.code;

    return codeMap;
}

void printPrefixTree(vector<Symbol>& symbols) {
    cout << "Символы, их частота появления и коды:\n";
    char temp;
    for (const Symbol& symbol : symbols) {
        string temp;
        (symbol.character == ' ') ? temp = "Пробел" : ((symbol.character == '\n') ? temp = "Перенос строки" : temp = "");
        if (temp.empty()) {
            cout << "'" << symbol.character << "'" << " -> Частота появления: " << symbol.frequency << ", Код: " << symbol.code << endl;
        }
        else {
            cout << temp << " -> Частота появления: " << symbol.frequency << ", Код: " << symbol.code << endl;
        }
    }
    cout << endl;
}

void printCode(const string& text, const unordered_map<char, string>& codeMap) {
    cout << "\nЗакодированная строка: ";
    for (char ch : text)
        cout << codeMap.at(ch);
    cout << endl;
}

string ParseFile(const string& name) {
    ifstream inputFile(name);
    if (!inputFile) {
        cerr << "Ошибка: не удалось открыть файл input.txt" << endl;
        return "";
    }

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();
    return text;
}

void calculateCompressionRatio(const string& text, const unordered_map<char, string>& codeMap) {
    int original_size = text.size() * 8;
    int encoded_size = 0;
    cout << "\nКоэффицент сжатия: ";
    for (char ch : text)
        encoded_size += codeMap.at(ch).size();
    cout << double(original_size) / double(encoded_size) << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string all_text = ParseFile("input.txt");
    vector<Symbol> symbols;

    unordered_map<char, string> codeMap = encodeShannonFano(all_text, symbols);

    printPrefixTree(symbols);

    calculateCompressionRatio(all_text, codeMap);

    return 0;
}
