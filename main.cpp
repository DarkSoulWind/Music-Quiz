#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include "header/quiz.h"
#include "header/player.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

const std::string JSONFILE = "songs.json";
const int TOPNUM = 5;
Question questionChosen;

void play(std::vector<Question>& questions) {
    bool stillPlaying = true;
    bool stillGuessing = true;
    int score = 0;
    int chances;
    int index;
    int addPoints;
    std::string name;
    std::string songChosen;
    std::string songGuess;
    std::vector<Player> players = get_players();

    std::cout << "Enter name for player: ";
    getline (std::cin, name);

    while (stillPlaying) {
        score = 0;
        chances = 2;
        while (chances >= 1) {
            chances = 2;

            if (questions.empty()) {
                std::cout << "\nWow! Looks like we're out of songs!\n";
                stillPlaying = false;
                break;
            } else {
                index = rand() % questions.size();
                questionChosen = questions[index];
                songChosen = questionChosen.Song;
                hash_out(songChosen); // hashes it out
                std::cout << "\n" << questions[index].Artist << "\n" << songChosen << std::endl;

                stillGuessing = true;
                while (stillGuessing) {
                    std::cout << "\nGuess the name of the song: ";
                    getline (std::cin, songGuess);
                    
                    upperCase(songGuess); // make the songGuess all in caps
                    if (strip(songGuess) == questionChosen.Song) {
                        std::cout << "Wow! You guessed correct!\n";
                        chances == 2 ? addPoints = 3 : addPoints = 1;
                        score += addPoints;
                        std::cout << "+" << addPoints << " point(s)!\nYou have a score of " << score << std::endl;
                        stillGuessing = false;
                    } else {
                        std::cout << "You guessed wrong!\n";
                        chances--;
                        if (chances >= 1) std::cout << "Try again! You have one more chance.\n";
                        else {
                            stillGuessing = false;
                            stillPlaying = false;
                        }
                        
                    }
                    questions.erase (questions.begin() + index); // removes question from list
                }
            }
        }
    }
    if (questions.empty()) std::cout << "You have managed to answer all correctly!\n";
    else {
        std::cout << "\nOut of chances\nThe real answer was " << questionChosen.Song << " by " << questionChosen.Artist << ".\n";
        std::cout << "Your final score is " << score << std::endl;
    }
    save_leaderboard(players, name, score);
    std::cout << "\nPress enter to return to the menu.\n";
    std::cin.get();
}

void game_instructions() {
    std::cout << "HOW TO PLAY:\n";
    std::cout << "This game plays simply really!\n";
    std::cout << "You are given an artist's name and the first letter of each word of one of their songs.\n";
    std::cout << "Your job is to guess the EXACT name of their song!\n";
    std::cout << "You will be given two chances to do this!\n";
    std::cout << "Answering in one guess will award you three points! Two guesses will only award you one point.\n";
    std::cout << "\nPress enter to return to the menu.\n";
    std::cin.get();
};

void leaderboard() {
    std::vector<Player> players = get_players();
    print_leaderboard(players, TOPNUM);
    std::cout << "\nPress enter to return to the menu.\n";
    std::cin.get();
};

int main() {
    bool run = true;
    int choice;
    std::vector<Question> questions;

    while(run) {
        choice = menu();
        system("clear");

        // flush the input buffer
        std::cin.clear();
        fflush(stdin);

        switch (choice) {
        case 1:
            questions = get_questions();
            play(questions);
            break;
        case 2:
            game_instructions();
            break;
        case 3:
            leaderboard();
            break;
        case 4:
            run = false;
            break;
        }
    }
    std::cout << "\nThanks for playing!\n";
}