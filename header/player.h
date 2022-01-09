#pragma once
#include <iostream>
#include <string>
#include <vector>

template<typename T>
void print_vector(std::vector<T>);

struct Player {
    std::string Name;
    int Score;
};

bool maxValue(Player, Player);

std::vector<Player> get_players();

void print_leaderboard(std::vector<Player>, int);

void save_leaderboard(std::vector<Player>&, std::string, int);

std::ostream& operator<<(std::ostream&, const Player&);