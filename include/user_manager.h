//
// Created by athar on 22-06-2025.
//

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#pragma once
#include <string>
#include <unordered_map>

class UserManager {
private:
    std::unordered_map<std::string, std::string> users;
public:
    void registerUser(const std::string &username, const std::string &password);
    bool authenticate(const std::string &username, const std::string &password);
};

#endif //USER_MANAGER_H
