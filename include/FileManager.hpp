#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include<vector>
#include<string>
#include "Player.hpp"

class FileManager
{
private:
    static void ensureFilesExist();
    static constexpr const char* DATA_DIR = "data/";
    static constexpr const char* USERS_FILE = "data/users.txt";
    static constexpr const char* HISTORY_FILE = "data/history.txt";
public:
    static std::vector<Player> loadUsers();
    static void saveUsers(const std::vector<Player>& users);

    static bool saveGame(const std::string& player1, const std::string& player2, const std::string& boardState);
    static std::string loadGame(const std::string& player1, const std::string& player2);
    static std::vector<std::string> getLastGames(int count = 10);
};
#endif