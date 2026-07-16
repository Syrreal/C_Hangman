#include <array>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <assert.h>
#include <iostream>

std::string wordFile = "words.txt";

class Hangman {
    private:
        static Hangman* mGame;
        char mGuessed[26] = {};
        char mAvailable[26];
        char mCorrect;
        char mIncorrect;
        char mWordWithCorrectGuesses;
        std::string mWord = "";
        int mWrongGuessesLeft = 5;

    public:
        // Potentially construct with different game types (eg. compound words or phrases)
        Hangman() {
            // We should only ever have one instance of the game at a time. If one is already set, return it
            for(int i = 65; i <= 90; i++) {
                mAvailable[i-65] = static_cast<char>(i);
            }
            mWord = pickWord();
            char mWordWithCorrectGuesses[(int)mWord.length()] = {};
            char mCorrect[(int)mWord.length()] = {};
            char mIncorrect[mWrongGuessesLeft] = {};
        }
        // OpenGL keypresses are an enum that correspond to the ASCII code, so we'll pass the keypress as an integer
        // We return if the guess was correct or not so that we can run an animation
        bool guessLetter(int letter) {
            // TODO: implement letter checking
            return false;
        };
        void printWord() {
            std::cout << "The word is: " << mWord << std::endl;
        };

    private:
        std::string pickWord() {
            std::ifstream file(wordFile);
            assert(file.is_open());
            // Count lines to set max for rand()
            auto count = std::count_if(std::istreambuf_iterator<char>{file}, {}, [](char c) { return c == '\n'; });
            std::cout << "Line count: " << count << std::endl;

            file.seekg(0);

            // Seed and pick a random number, bind it to the range set by count
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, (int)count);
            int r = distrib(gen);

            std::cout << "Picking word on line: " << r << std::endl;

            std::string word = "";
            for(; r >= 0; r--) {
                std::cout << "On line: " << (count - (count - r)) << std::endl;
                std::getline(file, word);
                std::cout << "Current word: " << word << std::endl;
            }
            file.close();
            return word;
        };

};


// TODO: remove after testing
int main() {
    Hangman game = Hangman{};
    game.printWord();
}