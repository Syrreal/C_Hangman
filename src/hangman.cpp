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
        };

        // OpenGL keypresses are an enum that correspond to the upper case ASCII code, so we'll pass the keypress as an integer
        // We return an int representing the guess result    0 = Incorrect, 1 = Correct, 2 = Letter already guessed
        int makeGuess(int letter_ascii) {
            char letter = static_cast<char>(letter_ascii);
            if(mGuessesLeft <= 0){
                std::cout << "Game is ended no more guessing" << std::endl;
                return 2;
            }
            int correctGuess = 2;
            for(int i = 0; i < std::size(mAvailable); i++) {
                // Check if the letter has already been guessed
                if(mAvailable[i] == letter) {
                    // Set the guessed letter to a lowercase z since it's unused by the rest of the game logic
                    mAvailable[i] = 'z';
                    correctGuess = checkGuess(letter);
                }
            }
            if(correctGuess == 2){
                std::cout << "Letter already guessed!" << std::endl;
                // Letter was already guessed
                return 2;
            } else if (correctGuess == 1){
                // Guess was incorrect, decrement guess counter and see if game is over
                mGuessesLeft--;
                if(mGuessesLeft <= 0){
                    std::cout << "End game" << std::endl; // TODO: Game over/Loss implementation
                    // No guesses left, end game
                }
                std::cout << "Continue" << std::endl;
                // Guesses remaining, return the the guess was incorrect and continue game
                return correctGuess;
            }
            // Guess was correct, check if all letters are filled and game should be over
            if(mWord.compare(mWordWithCorrectGuesses) == 0) {
                std::cout << "You win!" << std::endl; // TODO: Game over/Win implementation
                mGuessesLeft = 0; // Setting Guesses left to 0 on win for now since there no real game end
                return 2;
            }
            // Guess was correct and game is not over
            return correctGuess;
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
            std::cout << "Remaining guesses: " << mGuessesLeft << "\n";
            std::cout << "Available letters --- \n";
            for(char letter: mAvailable){
                std::cout << letter;
            } 
            std::cout << "\n-------- \n";
            std::cout << "Correct letters: " << formatted_guess << std::endl;
        };
        
        std::tuple<std::string*, char(*)[], int> getGameState() {
            return  {&mWordWithCorrectGuesses, &mAvailable, mGuessesLeft};
        }
    private:
        int checkGuess(char letter) {
            // Check if the letter is in the guess word
            std::vector<size_t> positions;
            size_t pos = mWord.find(letter, 0);
            while(pos != std::string::npos)
            {
                positions.push_back(pos);
                pos = mWord.find(letter, pos+1);
            }
            // If no positions were recorded the guess was wrong
            if (positions.size() == 0) {
                return 0;
            }
            // Iterate through the guess word template and fill in all the spots marked with the guessed letter
            for(int i = 0; i < positions.size(); i++) {
                // convert char to string to use replace function
                std::string l = {letter};
                mWordWithCorrectGuesses.replace(positions[i], 1, l);
            }
            return 1;
        }

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
    game.makeGuess(74);
    game.makeGuess(80);
    game.makeGuess(85);
    game.makeGuess(77);
    game.makeGuess(65);
    game.makeGuess(66);
    auto state = game.getGameState();
    std::cout << "Template word: " << *std::get<0>(state) << '\n';
    std::cout << "Letters left available: ";
    char* letters = *std::get<1>(state);
    for(int i = 0; i < 26; i++){
        if(letters[i] != 'z'){
            std::cout << letters[i];
        }
    }
    std::cout << "\n";
    std::cout << "Guesses left: " << std::get<2>(state) << '\n';
    game.makeGuess(67);
    game.makeGuess(74);
    game.makeGuess(68);
    game.makeGuess(69);
}