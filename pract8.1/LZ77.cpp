#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;

struct LZ77Triplet {
    int offset;
    int length;
    char nextChar;

    LZ77Triplet(int off, int len, char next) : offset(off), length(len), nextChar(next) {}
};

vector<LZ77Triplet> encodeLZ77(const string& input, int searchBufferSize, int lookaheadBufferSize) {
    vector<LZ77Triplet> encodedData;
    size_t i = 0;

    while (i < input.size()) {
        int matchOffset = 0;
        int matchLength = 0;
        char nextChar = '\0';

        int searchBufferStart = max(0, static_cast<int>(i) - searchBufferSize);
        string searchBuffer = input.substr(searchBufferStart, i - searchBufferStart);
        string lookaheadBuffer = input.substr(i, lookaheadBufferSize);

        for (size_t j = 0; j < searchBuffer.size(); ++j) {
            int length = 0;

            while (length < lookaheadBuffer.size() && searchBuffer[j + length] == lookaheadBuffer[length]) {
                ++length;

                if (j + length >= searchBuffer.size())
                    break;
            }

            if (length > matchLength) {
                matchLength = length;
                matchOffset = searchBuffer.size() - j;
            }
        }

        nextChar = (matchLength < lookaheadBuffer.size()) ? lookaheadBuffer[matchLength] : '\0';

        encodedData.emplace_back(matchOffset, matchLength, nextChar);

        i += matchLength + 1;
    }

    return encodedData;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string input = "0001000010101001101";
    int searchBufferSize = 5;
    int lookaheadBufferSize = 5;

    vector<LZ77Triplet> compressed = encodeLZ77(input, searchBufferSize, lookaheadBufferSize);

    cout << "Результат сжатия информации (offset, length, nextChar):" << endl;
    for (const auto& triplet : compressed) {
        cout << "(" << triplet.offset << ", " << triplet.length << ", "
            << (triplet.nextChar ? triplet.nextChar : '-') << ") ";
    }

    
    return 0;
}
