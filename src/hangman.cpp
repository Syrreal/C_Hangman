#include <array>
#include <string>
#include <vector>
#include <random>
#include <time.h>

std::string wordFile = "words.txt";

template <typename T, std::size_t N>
std::string pickWord() {
    std::ifstream file(wordFile);
    static_assert(file.is_open());
    // Count lines to set max for rand() and reset file pointer
    auto count = std::count_if(std::istreambuf_iterator<char>{file}, {}, [](char c) { return c == '\n'; });
    std::fseek(file, 0, SEEK_SET);
    // Seed and pick a random number
    srand( (unsinged)time(NULL));
    int r = rand(0, count);

    std::string word = "";
    for(; count => 0; count--) {
        std::getline(file, word, '\n');
    }
    return word;
};
