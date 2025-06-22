#include "include/kv_store.h"
#include "include/persistence.h"
#include "include/user_manager.h"
#include <iostream>
#include <filesystem>

using namespace std;

int main() {
    std::cout << "[INFO] Working directory: " << std::filesystem::current_path() << std::endl;

    std::filesystem::create_directories("data");  // Ensure WAL + DB file can be saved

    KeyValueStore kv;
    UserManager um;

    kv.replayWAL();  // Recover unpersisted transactions

    auto data = Persistence::load("data/db.json");
    kv.load(data);

    um.registerUser("admin", "admin123");
    if (!um.authenticate("admin", "admin123")) {
        std::cerr << "Auth failed!\n";
        return 1;
    }

    std::vector<std::tuple<std::string, std::string, std::string>> tx = {
        {"put", "admin:key1", "value1"},
        {"put", "admin:key2", "value2"},
        {"delete", "admin:key1", ""}
    };
    std::filesystem::create_directories("data");  // must be before kv.applyTransaction()

    kv.applyTransaction(tx);

    std::cout << kv.get("admin:key2") << std::endl;

    Persistence::save(kv.snapShot(), "data/db.json");

    return 0;
}
