#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "quiz.h"
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"

// compiles the contents of the json file into one string
std::string json_to_string(std::string JSONFILE) {
    std::ifstream FileHandle(JSONFILE);
    std::string text;
    std::string result;

    while (std::getline(FileHandle, text)) {
        result += text;
    }

    FileHandle.close();
    return result;
}

// retrieves artists and song names from the json file and compiles them into a vector
std::vector<Question> get_questions() {
    std::string result = json_to_string("songs.json");
    const char* json = result.c_str();

    // parse json
    rapidjson::Document d;
    d.Parse(json);

    // retrieve data from the json
    const rapidjson::Value& songs = d["songs"];
    assert(songs.IsArray());
    const rapidjson::Value& artists = d["artists"];
    assert(artists.IsArray());
    
    // create a vector of questions structs
    std::vector<Question> questions;
    Question q;
    for (rapidjson::SizeType i = 0; i<songs.Size(); i++) {
        q.Song = songs[i].GetString();
        q.Artist = artists[i].GetString();
        questions.push_back(q);
    }

    return questions;
}

// basic menu system
int menu() {
    system("clear");
    int choice;
    std::cout << "Welcome to my music quiz!\n1. Play\n2. Instructions\n3. Leaderboard\n4. Exit\nSelect an option: ";
    std::cin >> choice;

    while (!(choice >= 1 && choice <= 4)) {
        std::cout << "Please enter an integer between 1 and 4: ";
        std::cin >> choice;
    }

    return choice;
}

// splits up a string at each space into a vector
std::vector<std::string> split_up(std::string text) {
    std::vector<std::string> splitUpWords;
    std::string word = "";
    for (auto x : text) {
        if (x == ' ') {
            splitUpWords.push_back(word);
            word = "";
        } else {
            word = word + x;
        }
    }
    splitUpWords.push_back(word);
    return splitUpWords;
}

// removes whitespaces before and after a string
std::string strip(const std::string& inpt) {
    auto start_it = inpt.begin();
    auto end_it = inpt.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}

// converts whole string to uppercase
void upperCase(std::string& text) {
    std::for_each(text.begin(), text.end(), [](char & c) {
        c = toupper(c);
    });
}

// leaves only the first letter of each word
void hash_out(std::string& text) {
    std::vector<std::string> splitUpWords = split_up(text);
    std::string letter;
    text = "";
    for (int i=0; i<splitUpWords.size(); i++) {
        for (auto x : splitUpWords[i]) {
            if (x == splitUpWords[i][0]) letter = splitUpWords[i][0];
            else letter = "_";
            text += letter;
        }
        text += " ";
    }
}

std::ostream& operator<<(std::ostream& stream, const Question& question) {
    stream << "{'Artist': '" << question.Artist << "', 'Song': '" << question.Song << "'}";
    return stream;
}