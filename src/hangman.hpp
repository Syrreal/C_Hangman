#pragma once
#include <tuple>
#include <string>

class Hangman {
    int makeGuess(int letter_ascii);
    class std::tuple<std::string*, char(*)[], int> getGameState();
    void printWord();
    void printGuesses();
};