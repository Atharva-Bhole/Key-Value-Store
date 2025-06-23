#define CROW_MAIN
#include "crow/include/crow/app.h"
#include "crow/include/crow/json.h"
#include "include/kv_store.h"
#include "include/user_manager.h"
#include "include/persistence.h"
#include "include/nlohmann/json.hpp"

#include <filesystem>
#include <csignal>
using namespace std;

KeyValueStore kv;
UserManager um;
crow::SimpleApp app;

void shutdownHandler(int signum) {
    Persistence::save(kv.snapShot(), "data/db.json");
    kv.clearWAL();
    std::_Exit(0);
}

int main() {
    std::filesystem::create_directories("data");
    kv.replayWAL();
    auto data = Persistence::load("data/db.json");
    kv.load(data);
    std::signal(SIGINT, shutdownHandler);
    std::signal(SIGTERM, shutdownHandler);

    // GET value by key
    CROW_ROUTE(app, "/get/<string>")
    ([](const string& key) {
        if (!kv.exists(key))
            return crow::response(404, "Key not found");
        return crow::response(kv.get(key));
    });

    CROW_ROUTE(app, "/put").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("key") || !body.has("value"))
            return crow::response(400, "Invalid JSON");

        kv.put(body["key"].s(), body["value"].s());
        return crow::response(200, "Inserted");
    });

    CROW_ROUTE(app, "/delete/<string>").methods(crow::HTTPMethod::Delete)
    ([](const string& key) {
        if (!kv.exists(key))
            return crow::response(404, "Key not found");

        kv.del(key);
        return crow::response(200, "Deleted");
    });

    CROW_ROUTE(app, "/store").methods(crow::HTTPMethod::Post)
    ([] (const crow::request &req) {
        auto body_json = nlohmann::json::parse(req.body);
        for (auto & el: body_json.items()) {
            kv.put(el.key(), el.value().dump());
        }

        return crow::response(200, "Inserted the data successfully");
    });
    CROW_ROUTE(app, "/tx").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("ops") || body["ops"].t() != crow::json::type::List)
            return crow::response(400, "Expected 'ops' list");

        vector<tuple<string, string, string>> operations;

        for (const auto& op : body["ops"]) {
            if (!op.has("type") || !op.has("key"))
                return crow::response(400, "Each op needs 'type' and 'key'");
            string type = std::string(op["type"].s());
            string key = std::string(op["key"].s());
            string value = op.has("value") ? std::string(op["value"].s()) : "";

            operations.emplace_back(type, key, value);
        }

        kv.applyTransaction(operations);
        return crow::response(200, "Transaction Applied");
    });

    app.port(8080).multithreaded().run();

    return 0;
}
