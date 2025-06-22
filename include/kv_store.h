//
// Created by athar on 22-06-2025.
//

#ifndef KV_STORE_H
#define KV_STORE_H

#pragma once
#include<unordered_map>
#include<string>
#include<mutex>
#include <vector>

class KeyValueStore {
private:
    std::unordered_map<std::string, std::string> store;
    std::mutex storeMutex;
    std::string walFile = "data/wal.log";

public:
    std::string get(std::string key);
    void put(const std::string& key, const std::string &value);
    void del(std:: string key);
    bool exists(const std::string& key);
    std::unordered_map<std::string, std::string> snapShot();
    void load(std::unordered_map<std::string, std::string> &data);
    void applyTransaction(const std::vector<std::tuple<std::string, std::string, std::string>> &operations);
    void replayWAL();
    void clearWAL();
private:
    void writeToWAL(const std::vector<std::tuple<std::string, std::string, std::string>>& operations);
};

#endif //KV_STORE_H

