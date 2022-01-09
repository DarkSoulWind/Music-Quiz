#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include "player.h"
#include "quiz.h"

template<typename T>
void print_vector(std::vector<T> vector) {
    if (!vector.empty()) {
        for (auto i = vector.begin(); i<vector.end(); i++) {
            std::cout << *i << " ";
        }
        std::cout << "\n";
    }
}

bool maxValue(Player i, Player j) {
    return i.Score > j.Score;
}

std::vector<Player> get_players() {
    std::vector<Player> players;
    Player p;
    std::string result = json_to_string("songs.json");
    const char* json = result.c_str();

    //parse json
    rapidjson::Document d;
    d.Parse(json);

    // retrieve data from the json
    rapidjson::Value& high_scores = d["high_scores"];
    assert(high_scores.IsArray());
    rapidjson::Value& high_scores_names = d["high_scores_names"];
    assert(high_scores_names.IsArray());

    // converting all the high_scores and names into player structs and saving
    // them in a vector of players
    int size = high_scores.Size();
    for (rapidjson::SizeType i = 0; i < high_scores.Size(); i++) {
        p = {high_scores_names[i].GetString(), high_scores[i].GetInt()};
        players.push_back(p);
    }

    return players;
}

void print_leaderboard(std::vector<Player> players, int top_length) {
    std::cout << "TOP " << top_length << " HIGH SCORES:\n";
    if (!players.empty()) {
        for (int i = 0; i<top_length; i++) {
            if (i<players.size())
                std::cout << players[i].Score << " " << players[i].Name << "\n";
            else
                std::cout << "N/A\n";
        }
    } else std::cout << "Wow looks like we have no players :\\ \n";
}

void save_leaderboard(std::vector<Player>& players, std::string name, int score) {
    Player p = {name, score};
    players.push_back(p);
    std::sort (players.begin(), players.end(), &maxValue);

    std::string result = json_to_string("songs.json");
    const char* json = result.c_str();

    //parse json
    rapidjson::Document d;
    d.Parse(json);

    // retrieve data from the json
    rapidjson::Value& high_scores = d["high_scores"];
    assert(high_scores.IsArray());
    rapidjson::Value& high_scores_names = d["high_scores_names"];
    assert(high_scores_names.IsArray());
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    int size = high_scores.Size() + 1;

    // clearing the high_scores and names arrays
    high_scores_names.Clear();
    high_scores.Clear();

    // refilling the arrays with the sorted players vector
    std::string tempName;
    int tempScore;
    rapidjson::Value strVal;
    for (rapidjson::SizeType i = 0; i < size; i++) {
        tempName = players[i].Name;
        strVal.SetString(tempName.c_str(), tempName.length(), allocator);
        tempScore = players[i].Score;
        high_scores_names.PushBack(strVal, allocator);
        high_scores.PushBack(rapidjson::Value(tempScore), allocator);
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    std::ofstream FileHandle("songs.json");
    FileHandle << buffer.GetString();
    std::cout << "\nSaved results to leaderboard!\n";
    FileHandle.close();
}

std::ostream& operator<<(std::ostream& stream, const Player& player) {
    stream << "{'Name': '" << player.Name << "', 'Score': " << player.Score << "}";
    return stream;
}