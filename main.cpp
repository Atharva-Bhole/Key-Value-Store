//
// Created by athar on 22-06-2025.
//

#include "include/kv_store.h"
#include "include/persistence.h"
#include "include/user_manager.h"
#include<iostream>
using namespace std;

int main() {
    KeyValueStore kv;
    UserManager um;

    // Load from JSON
    auto data = Persistence::load("data/db.json");
    kv.load(data);

    um.registerUser("admin", "admin123");
    if (!um.authenticate("admin", "admin123")) {
        std::cerr << "Auth failed!\n";
        return 1;
    }

    // Sample transaction
    std::vector<std::tuple<std::string, std::string, std::string>> tx = {
        {"put", "admin:key1", "value1"},
        {"put", "admin:key2", "value2"},
        {"delete", "admin:key1", ""}
    };
    kv.applyTransaction(tx);

    std::cout << kv.get("admin:key2") << std::endl;

    Persistence::save(kv.snapShot(), "data/db.json");
    return 0;
}
