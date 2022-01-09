#pragma once
#include <vector>
#include <string>
#include <iostream>

struct Question {
    std::string Song;
    std::string Artist;
};

std::string json_to_string(std::string);

std::vector<Question> get_questions();

int menu();

std::vector<std::string> split_up(std::string);

std::string strip(const std::string&);

void upperCase(std::string&);

void hash_out(std::string&);

std::ostream& operator<<(std::ostream&, const Question&);