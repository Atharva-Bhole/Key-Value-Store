#include <fstream>
#include <ios>
#include <sstream>
#include "include/kv_store.h"
#include<iostream>
using namespace std;

string KeyValueStore::get(string key) {
    lock_guard<mutex> lock(storeMutex);
    return store.contains(key) ? store[key] : string("");
}

void KeyValueStore::put(const string& key, const string &value) {
    lock_guard<mutex> lock(storeMutex);
    store[key] = value;
}

void KeyValueStore::del(string key) {
    lock_guard<mutex> lock(storeMutex);
    store.erase(key);
}

bool KeyValueStore::exists(const std::string& key) {
    lock_guard<mutex> lock(storeMutex);
    return store.find(key) != store.end();
}

std::unordered_map<std::string, std::string> KeyValueStore::snapShot() {
    lock_guard<mutex> lock(storeMutex);
    return store;
}

void KeyValueStore::load(std::unordered_map<std::string, std::string> &data) {
    lock_guard<mutex> lock(storeMutex);
    store = data;
}

void KeyValueStore::applyTransaction(const std::vector<std::tuple<std::string, std::string, std::string> > &operations) {
    lock_guard<mutex> lock(storeMutex);
    writeToWAL(operations);
    for (const auto& op: operations) {
        const string& type = std::get<0>(op);
        const string& key = std::get<1>(op);
        const string& value = std::get<2>(op);

        if (type == "put") {
            store[key] = value;
        }else if (type == "delete") {
            store.erase(key);
        }
    }
}

void KeyValueStore::writeToWAL(const std::vector<std::tuple<std::string, std::string, std::string>>& operations) {
    std::ofstream wal(walFile, std::ios::app);
    if (!wal.is_open()) {
        std::cerr << "[ERROR] Could not open WAL file: " << walFile << std::endl;
        return;
    }
    std::cout << "[DEBUG] WAL file path: " << walFile << std::endl;

    for (const auto& op : operations) {
        wal << std::get<0>(op) << "," << std::get<1>(op) << "," << std::get<2>(op) << "\n";
    }
    wal.flush();  // 💡 optional: force flush to disk
    wal.close();
}



void KeyValueStore::replayWAL() {
    ifstream wal(walFile);
    string line;
    while (getline(wal, line)) {
        istringstream ss(line);
        string type, key, value;
        getline(ss, type, ',');
        getline(ss, key, ',');
        getline(ss, value, ',');
        if (type == "put") {
            store[key] = value;
        }else if (type == "delete") {
            store.erase(key);
        }
    }
}

void KeyValueStore::clearWAL() {
    ofstream wal(walFile);
    wal.close();
}


