#include "../include/FileManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include<filesystem>

void FileManager::ensureFilesExist() {
    if (!std::filesystem::exists(DATA_DIR)) {
        std::filesystem::create_directory(DATA_DIR);
        std::cout << "data/ directory created!\n";
    }
    
    std::ifstream usersFile(USERS_FILE);
    if (!usersFile.is_open()) {
        std::ofstream newUsers(USERS_FILE);
        newUsers << "1,Admin,admin,0\n";
        newUsers.close();
        std::cout << "data/users.txt created!\n";
    }
    usersFile.close();

    std::ifstream historyFile(HISTORY_FILE);
    if (!historyFile.is_open()) {
        std::ofstream newHistory(HISTORY_FILE);
        newHistory.close();
        std::cout << "data/history.txt created!\n";
    }
    historyFile.close();
}

std::vector<Player> FileManager::loadUsers() {
    ensureFilesExist();

    std::vector<Player> users;
    std::ifstream file(USERS_FILE);
    
    if (!file.is_open()) {
        std::cerr << "Error opening " << USERS_FILE << "!\n";
        return users;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, username, password, score;
        
        std::getline(ss, id, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, score, ',');
        
        users.push_back(Player(std::stoi(id), username, password, std::stoi(score)));
    }
    file.close();
    return users;
}

void FileManager::saveUsers(const std::vector<Player>& users) {
    ensureFilesExist();
    
    std::ofstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error saving " << USERS_FILE << "!\n";
        return;
    }
    
    for (const auto& user : users) {
        file << user.toString() << std::endl;
    }
    file.close();
}

bool FileManager::saveGame(const std::string& player1, const std::string& player2, 
                           const std::string& boardState) {
    ensureFilesExist();
    
    std::ofstream file(HISTORY_FILE, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error saving " << HISTORY_FILE << "!\n";
        return false;
    }
    
    file << player1 << "," << player2 << "," << boardState << std::endl;
    file.close();
    return true;
}

std::string FileManager::loadGame(const std::string& player1, const std::string& player2) {
    ensureFilesExist();
    
    std::ifstream file(HISTORY_FILE);
    if (!file.is_open()) {
        std::cerr << "Error opening " << HISTORY_FILE << "!\n";
        return "";
    }

    std::string line;
    std::vector<std::string> games;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string p1, p2, board;
        std::getline(ss, p1, ',');
        std::getline(ss, p2, ',');
        std::getline(ss, board);

        if ((p1 == player1 && p2 == player2) || (p1 == player2 && p2 == player1)) {
            if (board.find("End") == std::string::npos) {
                games.push_back(line);
            }
        }
    }
    file.close();

    if (!games.empty()) {
        std::string lastGame = games.back();
        std::stringstream ss(lastGame);
        std::string p1, p2, board;
        std::getline(ss, p1, ',');
        std::getline(ss, p2, ',');
        std::getline(ss, board);
        return board;
    }
    return "";
}

std::vector<std::string> FileManager::getLastGames(int count) {
    ensureFilesExist();
    
    std::vector<std::string> games;
    std::ifstream file(HISTORY_FILE);
    
    if (!file.is_open()) {
        std::cerr << "Error opening " << HISTORY_FILE << "!\n";
        return games;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            games.push_back(line);
        }
    }
    file.close();

    int start = std::max(0, static_cast<int>(games.size()) - count);
    std::vector<std::string> lastGames;
    for (int i = start; i < static_cast<int>(games.size()); i++) {
        lastGames.push_back(games[i]);
    }
    return lastGames;
}