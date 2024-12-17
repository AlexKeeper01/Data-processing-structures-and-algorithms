#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <cmath>
#include <set>

using namespace std;

struct Symbol {
    char character;
    int frequency;
    double probability;
    string code;
};

struct Node {
    int pos;
    char character;
    int frequency;
    Node* left;
    Node* right;

    Node(char ch, int freq, int p) : character(ch), frequency(freq), left(nullptr), right(nullptr), pos(p) {}
};

void assignCodes(Node* root, unordered_map<char, string>& huffmanCodes, string code = "") {
    if (!root) return;

    if (root->character != '\0') {
        huffmanCodes[root->character] = code;
    }

    assignCodes(root->left, huffmanCodes, code + "0");
    assignCodes(root->right, huffmanCodes, code + "1");
}

string compressString(const string& input, const unordered_map<char, string>& huffmanCodes) {
    string compressed;
    for (char ch : input) {
        compressed += huffmanCodes.at(ch);
    }
    return compressed;
}

string encodeHuffman(const string& input, vector<Symbol>& symbols) {
    vector<Node*> nodes;
    for (const auto& symbol : symbols) {
        nodes.push_back(new Node(symbol.character, symbol.frequency, 0));
    }

    for (auto& node : nodes) {
        node->pos = input.find(node->character);
    }

    while (nodes.size() > 1) {
        sort(nodes.begin(), nodes.end(), [&](Node* a, Node* b) {
            if (a->frequency == b->frequency) {
                return a->pos > b->pos;
            }
            else {
                return a->frequency < b->frequency;
            }});

            Node* left = nodes[0];
            Node* right = nodes[1];

            Node* combined = new Node('\0', left->frequency + right->frequency, 0);
            combined->left = left;
            combined->right = right;
            combined->pos = max(left->pos, right->pos);
            nodes.erase(nodes.begin(), nodes.begin() + 2);
            nodes.push_back(combined);
    }

    unordered_map<char, string> huffmanCodes;
    assignCodes(nodes[0], huffmanCodes);
    for (auto& symbol : symbols) {
        symbol.code = huffmanCodes.at(symbol.character);
    }
    return compressString(input, huffmanCodes);
}

vector<Symbol> calculateFreqAndProb(const string& input) {
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
        symbol.probability = static_cast<double>(pair.second) / total_chars;
        symbols.push_back(symbol);
    }

    return symbols;
}

void printTableWithCodes(vector<Symbol>& symbols, const string& input) {
    cout << setw(9) << "Символ" << setw(11) << "Частота" << setw(16) << "Вероятность" << setw(20) << "Код" << endl;
    cout << string(60, '-') << endl;

    sort(symbols.begin(), symbols.end(), [&](const Symbol& a, const Symbol& b) {
        if (a.probability == b.probability) {
            if ((a.code).size() == (b.code).size()) {
                return input.find(a.character) < input.find(b.character);
            }
            else {
                return (a.code).size() < (b.code).size();
            }
        }
        else {
            return a.probability > b.probability;
        }});

        for (auto& symbol : symbols) {
            cout << setw(5) << "'" << symbol.character << "'"
                << setw(10) << symbol.frequency
                << setw(15) << symbol.probability
                << setw(24) << symbol.code << endl;
        }
}

void calculateCompressionRation(const string& input, vector<Symbol>& symbols) {
    int ASCII_length = input.size() * 8;
    set<char> uniqueChars;
    for (char c : input)
        uniqueChars.insert(c);
    int Uniform_length = input.size() * (log(uniqueChars.size()) / log(2));
    int Encoded_length = encodeHuffman(input, symbols).size();
    cout << "Коэффицент сжатия относительно кодировки ASCII:\n";
    cout << double(ASCII_length) / Encoded_length << endl;
    cout << "Коэффицент сжатия относительно равномерного кода:\n";
    cout << double(Uniform_length) / Encoded_length << endl;
}

void calculateAvgLengthAndDispersion(vector<Symbol>& symbols) {
    double averageLength = 0;
    double dispersion = 0;
    for (auto& symbol : symbols) {
        averageLength += symbol.probability * (symbol.code).size();
    }
    for (auto& symbol : symbols) {
        dispersion += symbol.probability * pow((symbol.code).size() - averageLength, 2);
    }
    cout << "Средняя длина кода: " << averageLength << endl << "Дисперсия кода: " << dispersion << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string input = "попов алексей валерьевич";
    vector<Symbol> symbols = calculateFreqAndProb(input);

    string encodedText = encodeHuffman(input, symbols);

    printTableWithCodes(symbols, input);

    cout << endl;

    calculateAvgLengthAndDispersion(symbols);

    return 0;
}
