#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct Symbol {
    char character;
    int frequency;
    double probability;
    string code;
};

struct Node {
    char character;
    int frequency;
    Node* left;
    Node* right;

    Node(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

Node* buildHuffmanTree(vector<Symbol>& symbols) {
    vector<Node*> nodes;
    for (const auto& symbol : symbols) {
        nodes.push_back(new Node(symbol.character, symbol.frequency));
    }

    while (nodes.size() > 1) {
        sort(nodes.begin(), nodes.end(), [](Node* a, Node* b) {return a->frequency < b->frequency;});

        Node* left = nodes[0];
        Node* right = nodes[1];

        Node* combined = new Node('\0', left->frequency + right->frequency);
        combined->left = left;
        combined->right = right;

        nodes.erase(nodes.begin(), nodes.begin() + 2);
        nodes.push_back(combined);
    }

    return nodes[0];
}

// Обход дерева для назначения кодов символам
void assignCodes(Node* root, unordered_map<char, string>& huffmanCodes, string code = "") {
    if (!root) return;

    // Если это лист, сохранить код
    if (root->character != '\0') {
        huffmanCodes[root->character] = code;
    }

    assignCodes(root->left, huffmanCodes, code + "0");
    assignCodes(root->right, huffmanCodes, code + "1");
}

// Функция сжатия строки
string compressString(const string& input, const unordered_map<char, string>& huffmanCodes) {
    string compressed;
    for (char ch : input) {
        compressed += huffmanCodes.at(ch);
    }
    return compressed;
}

void printTableWithCodes(vector<Symbol>& symbols, const unordered_map<char, string>& huffmanCodes) {
    cout << setw(9) << "Символ" << setw(11) << "Частота" << setw(16) << "Вероятность" << setw(20) << "Код" << endl;
    cout << string(60, '-') << endl;

    sort(symbols.begin(), symbols.end(), [&](const Symbol& a, const Symbol& b) {return a.probability > b.probability; });

    for (auto& symbol : symbols) {
        symbol.code = huffmanCodes.at(symbol.character);
        cout << setw(5) << "'" << symbol.character << "'"
            << setw(10) << symbol.frequency
            << setw(15) << symbol.probability
            << setw(24) << symbol.code << endl;
    }
}

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

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string input = "попов алексей александрович";

    vector<Symbol> symbols = calculateFrequencies(input);

    Node* root = buildHuffmanTree(symbols);

    // Назначить коды символам
    unordered_map<char, string> huffmanCodes;
    assignCodes(root, huffmanCodes);

    // Сжать строку
    string compressed = compressString(input, huffmanCodes);

    // Вывести таблицу символов с кодами
    printTableWithCodes(symbols, huffmanCodes);

    cout << "\nСжатая строка: " << compressed << endl;

    return 0;
}
