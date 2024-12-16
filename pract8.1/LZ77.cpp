#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Структура для хранения триплета LZ77
struct LZ77Triplet {
    int offset;      // Смещение от текущей позиции
    int length;      // Длина совпадения
    char nextChar;   // Следующий символ

    // Конструктор для удобства создания объектов
    LZ77Triplet(int off, int len, char next) : offset(off), length(len), nextChar(next) {}
};

// Алгоритм сжатия LZ77
vector<LZ77Triplet> lz77_compress(const string& data, int searchBufferSize, int lookaheadBufferSize) {
    vector<LZ77Triplet> compressed;
    size_t i = 0; // Текущая позиция в строке

    while (i < data.size()) {
        int matchOffset = 0;
        int matchLength = 0;
        char nextChar = '\0';

        // Определяем границы буфера поиска
        int searchBufferStart = max(0, static_cast<int>(i) - searchBufferSize);
        string searchBuffer = data.substr(searchBufferStart, i - searchBufferStart);
        string lookaheadBuffer = data.substr(i, lookaheadBufferSize);

        // Ищем самое длинное совпадение
        for (size_t j = 0; j < searchBuffer.size(); ++j) {
            int length = 0;

            // Проверяем совпадение символов
            while (length < lookaheadBuffer.size() &&
                searchBuffer[j + length] == lookaheadBuffer[length]) {
                ++length;

                // Прекращаем, если выход за пределы searchBuffer
                if (j + length >= searchBuffer.size())
                    break;
            }

            // Если найдено более длинное совпадение
            if (length > matchLength) {
                matchLength = length;
                matchOffset = searchBuffer.size() - j;
            }
        }

        // Определяем следующий символ, если он есть
        nextChar = (matchLength < lookaheadBuffer.size()) ? lookaheadBuffer[matchLength] : '\0';

        // Добавляем триплет (offset, length, nextChar)
        compressed.emplace_back(matchOffset, matchLength, nextChar);

        // Сдвигаем окно
        i += matchLength + 1;
    }

    return compressed;
}

// Тестирование алгоритма
int main() {
    string data = "0001000010101001101";
    int searchBufferSize = 6; // Размер буфера поиска
    int lookaheadBufferSize = 6; // Размер буфера просмотра

    // Сжимаем строку
    vector<LZ77Triplet> compressed = lz77_compress(data, searchBufferSize, lookaheadBufferSize);

    // Вывод результата
    cout << "Compressed data (offset, length, nextChar):" << endl;
    for (const auto& triplet : compressed) {
        cout << "(" << triplet.offset << ", " << triplet.length << ", "
            << (triplet.nextChar ? triplet.nextChar : ' ') << ")" << endl;
    }

    return 0;
}
