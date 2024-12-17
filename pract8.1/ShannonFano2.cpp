#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <Windows.h>

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

string encodeShannonFano(const string& originalText, vector<Symbol>& symbols) {
    unordered_map<char, int> frequencyMap;
    string result;
    for (char ch : originalText)
        frequencyMap[ch]++;

    for (const auto& pair : frequencyMap)
        symbols.push_back({ pair.first, pair.second, "" });

    sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {return a.frequency > b.frequency; });

    shannonFano(symbols, 0, symbols.size() - 1);

    unordered_map<char, string> symbolMap;
    for (const Symbol& symbol : symbols) {
        symbolMap[symbol.character] = symbol.code;
    }

    for (char ch : originalText) {
        for (const Symbol& symbol : symbols) {
            if (symbol.character == ch) {
                result += symbol.code;
            }
        }
    }

    return result;
}

string decodeShannonFano(const string& encodedText, vector<Symbol>& symbols) {
    unordered_map<string, char> reverseCodeMap;
    for (const Symbol& symbol : symbols) {
        reverseCodeMap[symbol.code] = symbol.character;
    }

    string decodedText;
    string currentCode;

    for (char bit : encodedText) {
        currentCode += bit;

        if (reverseCodeMap.find(currentCode) != reverseCodeMap.end()) {
            decodedText += reverseCodeMap[currentCode];
            currentCode.clear();
        }
    }

    return decodedText;
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

void calculateCompressionRatio(const string& originalText, const string& encodedText) {
    int original_size = originalText.size() * 8;
    int encoded_size = encodedText.size();
    cout << "\nКоэффицент сжатия: " << endl;
    cout << double(original_size) / double(encoded_size) << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string originalText = ParseFile("input.txt");
    vector<Symbol> symbols;

    string encodedText = encodeShannonFano(originalText, symbols);

    printPrefixTree(symbols);

    calculateCompressionRatio(originalText, encodedText);

    cout << decodeShannonFano(encodedText, symbols);

    return 0;
}
