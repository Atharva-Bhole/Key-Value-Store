//
// Created by athar on 22-06-2025.
//
#include "include/persistence.h"
#include "include/nlohmann/json.hpp"
#include<fstream>

using json = nlohmann::json;
using namespace std;

void Persistence::save(const unordered_map<string, string> &data, const string& filename) {
    json j(data);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    }
}

unordered_map<string, string> Persistence::load(const string& filename) {
    unordered_map<string, string> data;
    ifstream file(filename);
    if (file.is_open()) {
        json j;
        file >> j;
        data = j.get<unordered_map<string, string>>();
        file.close();
    }
    return data;
}