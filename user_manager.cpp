//
// Created by athar on 22-06-2025.
//

#include "include/user_manager.h"

void UserManager::registerUser(const std::string &username, const std::string &password) {
    users[username] = password;
}

bool UserManager::authenticate(const std::string &username, const std::string &password) {
    return users.count(username) && users[username] == password;
}
