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
        char mGuessed[26] = {};
        char mAvailable[26];
        std::string mWordWithCorrectGuesses = "";
        std::string mWord = "";
        int mGuessesLeft = 7;
        
    public:
        // Potentially construct with different game types (eg. compound words or phrases)
        Hangman() {
            // Set available letters - uppercase A-Z
            for(int i = 65; i <= 90; i++) {
                mAvailable[i-65] = static_cast<char>(i);
            }
            mWord = pickWord();
            // Fill template word with z's to correct length
            int i = 0;
            for(char a: mWord) {
                mWordWithCorrectGuesses.append("z");
            }
        }
        // OpenGL keypresses are an enum that correspond to the upper case ASCII code, so we'll pass the keypress as an integer
        // We return an int representing the guess result    0 = Correct, 1 = Incorrect, 2 = Letter already guessed 
        int guessLetter(int letter_ascii) {
            char letter = static_cast<char>(letter_ascii);
            for(int i = 0; i < std::size(mAvailable); i++) {
                // Check if the letter has already been guessed
                if(mAvailable[i] == letter) {
                    mGuessed[i] == letter;
                    // Set the guessed letter to a lowercase z since it's unused by the rest of the game logic
                    mAvailable[i] = 'z';
                    // Check if the letter is in the guess word
                    std::vector<size_t> positions;
                    size_t pos = mWord.find(letter, 0);
                    while(pos != std::string::npos)
                    {
                        positions.push_back(pos);
                        pos = mWord.find(letter, pos+1);
                    }
                    // If no positions were recorded the guess was wrong and we can subtract a guess
                    if (positions.size() == 0) {
                        mGuessesLeft--;
                        return 1;
                    }
                    // Iterate through the guess word template and fill in all the spots marked with the guessed letter
                    for(int i = 0; i < positions.size(); i++) {
                        // convert char to string to use replace function
                        std::string l = {letter};
                        mWordWithCorrectGuesses.replace(1, 1, l);
                    }
                    break;
                    return 0;
                }
                else {
                    return 2;
                }
            }
            assert(true && "How'd you get here");
            return 2;
        };
        void printWord() {
            std::cout << "The word is: " << mWord << std::endl;
        };
        void printGuesses() {
            std::string str_avail = "";
            for(int i = 0; i < 25; i++) {
                str_avail = str_avail + mAvailable[i];
            }
            std::string formatted_guess = "";
            for(int i = 0; i < mWord.length(); i++) {
                if(mWordWithCorrectGuesses[i] == 'z') {
                    formatted_guess += '_';
                } else {
                    formatted_guess += mWordWithCorrectGuesses[i];
                }
            }
            std::cout << "Available letters --- \n" << mAvailable << "\n-------- \n";
            std::cout << "Correct letters: " << formatted_guess << "\n";
            std::cout << "Remaining guesses: " << mGuessesLeft << std::endl;
        };

    private:
        std::string pickWord() {
            std::ifstream file(wordFile);
            assert(file.is_open());
            // Count lines to set max for rand()
            auto count = std::count_if(std::istreambuf_iterator<char>{file}, {}, [](char c) { return c == '\n'; });

            file.seekg(0);

            // Seed and pick a random number, bind it to the range set by count
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, (int)count);
            int r = distrib(gen);

            std::string word = "";
            for(; r >= 0; r--) {
                std::getline(file, word);
            }
            file.close();
            // convert to uppercase
            for(auto & c: word){
                c = toupper(c);
            }
            return word;
        };

};


// TODO: remove after testing
int main() {
    Hangman game = Hangman{};
    game.printWord();
    game.printGuesses();
    game.guessLetter(65);
    game.printGuesses();
}