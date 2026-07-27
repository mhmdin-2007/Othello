#include "../include/Menu.hpp"
#include "../include/InputHandler.hpp"
#include "../include/FileManager.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>


Menu::Menu() : currentUserId(-1), isLoggedIn(false) {
    users = FileManager::loadUsers();
}


void Menu::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Menu::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

Player* Menu::getUserById(int id) {
    if (id >= 0 && id < static_cast<int>(users.size())) {
        return &users[id];
    }
    return nullptr;
}


void Menu::registerUser() {
    clearScreen();
    std::cout << "========== Register ==========\n";

    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    
    if (!InputHandler::isValidUsername(username)) {
        InputHandler::displayErrorMessage("Username must be at least 3 characters!");
        waitForEnter();
        return;
    }

    for (const auto& user : users) {
        if (user.getUsername() == username) {
            InputHandler::displayErrorMessage("Username already exists!");
            waitForEnter();
            return;
        }
    }

    std::cout << "Password: ";
    std::cin >> password;

    if (!InputHandler::isValidPassword(password)) {
        InputHandler::displayErrorMessage("Password must be at least 3 characters!");
        waitForEnter();
        return;
    }

    int newId = users.empty() ? 1 : users.back().getId() + 1;
    users.push_back(Player(newId, username, password, 0));
    FileManager::saveUsers(users);

    InputHandler::displaySuccessMessage("Registration successful! ID: " + std::to_string(newId));
    waitForEnter();
}

void Menu::loginUser() {
    clearScreen();
    std::cout << "========== Login ==========\n";

    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].getUsername() == username && users[i].getPassword() == password) {
            currentUserId = static_cast<int>(i);
            isLoggedIn = true;
            InputHandler::displaySuccessMessage("Welcome " + username + "!");
            waitForEnter();
            return;
        }
    }

    InputHandler::displayErrorMessage("Invalid username or password!");
    waitForEnter();
}

void Menu::logoutUser() {
    currentUserId = -1;
    isLoggedIn = false;
    InputHandler::displaySuccessMessage("Logged out successfully!");
    waitForEnter();
}

void Menu::playGame() {
    if (!isLoggedIn || currentUserId < 0) {
        InputHandler::displayErrorMessage("Please login first!");
        waitForEnter();
        return;
    }

    clearScreen();
    std::cout << "========== Play Game ==========\n";
    std::cout << "1. Play with Computer\n";
    std::cout << "2. Play with Another Player\n";
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        playWithBot();
    } else if (choice == 2) {
        playWithHuman();
    } else {
        InputHandler::displayErrorMessage("Invalid choice!");
        waitForEnter();
    }
}

void Menu::playWithBot() {
    clearScreen();
    std::cout << "========== Play with Computer ==========\n";
    std::cout << "Difficulty Level:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cout << "Choice: ";
    
    int level;
    std::cin >> level;

    if (level < 1 || level > 3) {
        InputHandler::displayErrorMessage("Invalid level! Using Easy.");
        level = 1;
    }

    std::string botName = "Bot";
    GameManager game;
    if (!game.startGame(getUserById(currentUserId)->getUsername(), botName, true, level)) {
        InputHandler::displayErrorMessage("Failed to start game!");
        waitForEnter();
    }
}

void Menu::playWithHuman() {
    clearScreen();
    std::cout << "========== Play with Another Player ==========\n";

    std::string opponentUsername, opponentPassword;
    std::cout << "Opponent Username: ";
    std::cin >> opponentUsername;
    std::cout << "Opponent Password: ";
    std::cin >> opponentPassword;

    bool found = false;
    for (const auto& user : users) {
        if (user.getUsername() == opponentUsername && user.getPassword() == opponentPassword) {
            found = true;
            break;
        }
    }

    if (!found) {
        InputHandler::displayErrorMessage("Invalid opponent credentials!");
        waitForEnter();
        return;
    }

    GameManager game;
    if (!game.startGame(getUserById(currentUserId)->getUsername(), opponentUsername, false)) {
            InputHandler::displayErrorMessage("Failed to start game!");
            waitForEnter();
    }
}

void Menu::deleteUser() {
    clearScreen();
    std::cout << "========== Delete User ==========\n";

    std::cout << "Admin Username: ";
    std::string adminUser;
    std::cin >> adminUser;
    std::cout << "Admin Password: ";
    std::string adminPass;
    std::cin >> adminPass;

    if (adminUser != "Admin" || adminPass != "admin") {
        InputHandler::displayErrorMessage("Access denied!");
        waitForEnter();
        return;
    }

    std::cout << "Username to delete: ";
    std::string username;
    std::cin >> username;

    if (username == "Admin") {
        InputHandler::displayErrorMessage("Cannot delete Admin!");
        waitForEnter();
        return;
    }

    auto it = std::remove_if(users.begin(), users.end(),
        [&](const Player& p) { return p.getUsername() == username; });

    if (it != users.end()) {
        users.erase(it, users.end());
        FileManager::saveUsers(users);
        InputHandler::displaySuccessMessage("User deleted successfully!");
    } else {
        InputHandler::displayErrorMessage("User not found!");
    }

    waitForEnter();
}

void Menu::showLastGames() {
    clearScreen();
    std::cout << "========== Last Games ==========\n";

    std::vector<std::string> games = FileManager::getLastGames(10);
    if (games.empty()) {
        std::cout << "No games recorded.\n";
    } else {
        std::cout << "Last " << games.size() << " games:\n\n";
        for (int i = static_cast<int>(games.size()) - 1; i >= 0; i--) {
            std::stringstream ss(games[i]);
            std::string p1, p2, rest;
            std::getline(ss, p1, ',');
            std::getline(ss, p2, ',');
            std::getline(ss, rest);

            std::cout << games.size() - i << ". " << p1 << " vs " << p2;
            if (rest.find("End") != std::string::npos) {
                std::cout << " - " << rest.substr(rest.find(",") + 1) << std::endl;
            } else {
                std::cout << " - In progress (saved)" << std::endl;
            }
        }
    }

    waitForEnter();
}

void Menu::showBestScores() {
    clearScreen();
    std::cout << "========== Best Scores ==========\n";

    std::vector<Player> sortedUsers = users;
    std::sort(sortedUsers.begin(), sortedUsers.end(),
        [](const Player& a, const Player& b) { return a.getBestScore() > b.getBestScore(); });

    std::cout << "Rank\tUsername\tScore\n";
    std::cout << "--------------------------------\n";
    int rank = 1;
    for (const auto& user : sortedUsers) {
        if (user.getUsername() != "Admin") {
            std::cout << rank++ << "\t" << user.getUsername() << "\t\t" << user.getBestScore() << std::endl;
        }
    }

    waitForEnter();
}


void Menu::showMainMenu() {
    while (true) {
        clearScreen();

        std::cout << "===================================\n";
        std::cout << "        OTHELLO GAME\n";
        std::cout << "===================================\n";
        
        if (isLoggedIn && currentUserId >= 0) {
            std::cout << "Logged in as: " << getUserById(currentUserId)->getUsername() << "\n";
        }
        
        std::cout << "===================================\n";
        std::cout << "1. Register User\n";
        std::cout << "2. Login to Game\n";
        
        if (isLoggedIn) {
            std::cout << "3. Logout\n";
            std::cout << "4. Play Game\n";
            std::cout << "5. Show Last Games\n";
            std::cout << "6. Show Best Scores\n";
            std::cout << "7. Exit Game\n";
        } else {
            std::cout << "3. Delete User (Admin only)\n";
            std::cout << "4. Show Last Games\n";
            std::cout << "5. Show Best Scores\n";
            std::cout << "6. Exit Game\n";
        }
        
        std::cout << "===================================\n";
        std::cout << "Your choice: ";

        int choice;
        std::cin >> choice;

        if (isLoggedIn) {
            switch (choice) {
                case 1: registerUser(); break;
                case 2: loginUser(); break;
                case 3: logoutUser(); break;
                case 4: playGame(); break;
                case 5: showLastGames(); break;
                case 6: showBestScores(); break;
                case 7:
                    std::cout << "Goodbye!\n";
                    return;
                default:
                    InputHandler::displayErrorMessage("Invalid choice!");
                    waitForEnter();
            }
        } else {
            switch (choice) {
                case 1: registerUser(); break;
                case 2: loginUser(); break;
                case 3: deleteUser(); break;
                case 4: showLastGames(); break;
                case 5: showBestScores(); break;
                case 6:
                    std::cout << "Goodbye!\n";
                    return;
                default:
                    InputHandler::displayErrorMessage("Invalid choice!");
                    waitForEnter();
            }
        }
    }
}