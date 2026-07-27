#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include "player.hpp"
#include "GameManager.hpp"

class Menu {
private:
    std::vector<Player> users;
    int currentUserId;
    bool isLoggedIn;

    void clearScreen() const;
    void waitForEnter() const;
    Player* getUserById(int id);

    void registerUser();
    void loginUser();
    void logoutUser();
    void playGame();
    void playWithBot();
    void playWithHuman();
    void deleteUser();
    void showLastGames();
    void showBestScores();

public:
    Menu();
    void showMainMenu();
};

#endif 