//
// Created by athar on 22-06-2025.
//

#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#pragma once
#include<unordered_map>
#include<string>
class Persistence {
public:
    static void save(const std::unordered_map<std::string, std::string> &data, const std::string &filename);
    static std::unordered_map<std::string, std::string> load(const std::string &filename);
};

#endif //PERSISTENCE_H




