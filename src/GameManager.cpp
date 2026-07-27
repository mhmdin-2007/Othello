#include "../include/GameManager.hpp"
#include "../include/InputHandler.hpp"
#include "../include/FileManager.hpp"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

GameManager::GameManager()
    : isBotGame(false)
    , bot(Difficulty::EASY)
    , gameEnded(false)
    , isGameSaved(false) {}

void GameManager::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void GameManager::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void GameManager::displayGameInfo() const {
    board.display();
    
    Color current = board.getCurrentPlayer();
    std::string currentName = (current == Color::BLACK) ? player1Name : player2Name;
    
    std::cout << "\nTurn: " << currentName 
              << " (" << (current == Color::BLACK ? "Black" : "White") << ")\n";
    
    auto moves = board.getValidMoves(current);
    InputHandler::displayValidMoves(moves);
}

bool GameManager::processPlayerMove(Color current, const std::string& input) {
    if (input == "q" || input == "Q") {
        std::cout << "\n💾 Game saved. You can continue later.\n";
        FileManager::saveGame(player1Name, player2Name, board.serialize());
        gameEnded = true;
        isGameSaved = true;
        return false;
    }
    
    int row, col;
    if (!InputHandler::parsePosition(input, row, col)) {
        InputHandler::displayErrorMessage("Invalid format! Use e.g., 'e3'");
        return false;
    }
    
    if (!board.isValidMove(row, col, current)) {
        InputHandler::displayErrorMessage("Invalid move! Try again.");
        return false;
    }
    
  
    board.makeMove(row, col, current);
    FileManager::saveGame(player1Name, player2Name, board.serialize());
    board.switchPlayer();
    
    return true;
}

bool GameManager::handleBotTurn(Color current) {
    if (!isBotGame) {
        return false;
    }
    
    std::string botName = (current == Color::BLACK) ? player1Name : player2Name;
    if (botName != player2Name) {
        return false;
    }
    
    std::cout << "\n🤖 Computer's turn (" << (current == Color::BLACK ? "Black" : "White") << ")...\n";
    
    // یه مکث برای طبیعی‌ تر شدن
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::pair<int,int> move = bot.getMove(board, current);
    if (move.first != -1) {
        board.makeMove(move.first, move.second, current);
        FileManager::saveGame(player1Name, player2Name, board.serialize());
        
        std::cout << "Computer played: " 
                  << char(move.second + 'a') << (move.first + 1) << std::endl;
        
        board.switchPlayer();
    }
    return true;
}

bool GameManager::endGame() {
    clearScreen();
    board.display();
    
    Color winner = board.getWinner();
    std::cout << "\n========== Game Over! ==========\n";
    
    if (winner == Color::DRAW) {
        std::cout << "It's a draw!\n";
    } else {
        std::string winnerName = (winner == Color::BLACK) ? player1Name : player2Name;
        std::cout << "Winner: " << winnerName << "!\n";
    }
    
    std::cout << "Final Score: Black " << board.getBlackCount() 
              << " - White " << board.getWhiteCount() << std::endl;
    
    std::vector<Player> users = FileManager::loadUsers();
    int finalScore = std::max(board.getBlackCount(), board.getWhiteCount());
    
    for (auto& user : users) {
        if (user.getUsername() == player1Name || user.getUsername() == player2Name) {
            if (finalScore > user.getBestScore()) {
                user.setBestScore(finalScore);
                std::cout << "New best score for " << user.getUsername() << "!\n";
            }
        }
    }
    FileManager::saveUsers(users);
    

    std::string result;
    if (winner == Color::DRAW) {
        result = "Draw";
    } else if (winner == Color::BLACK) {
        result = player1Name;
    } else {
        result = player2Name;
    }
    
    std::string score = std::to_string(board.getBlackCount()) + "-" + 
                        std::to_string(board.getWhiteCount());
    FileManager::saveGame(player1Name, player2Name, "End," + result + "," + score);
    
    waitForEnter();
    return true;
}

bool GameManager::checkAndHandleGameOver() {
    if (board.isGameOver()) {
        gameEnded = true;
        endGame();
        return true;
    }
    return false;
}


bool GameManager::startGame(const std::string& p1, const std::string& p2, 
                            bool botMode, int botLevel) {
    player1Name = p1;
    player2Name = p2;
    isBotGame = botMode;
    bot.setLevel(static_cast<Difficulty>(botLevel));
    board.initialize();
    gameEnded = false;
    isGameSaved = false;
    
    std::string savedBoard = FileManager::loadGame(p1, p2);
    if (!savedBoard.empty()) {
        std::cout << "\nSaved game found! Continue? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            board.deserialize(savedBoard);
            InputHandler::displaySuccessMessage("Game loaded successfully!");
        }
        waitForEnter();
    }
    
    return playGame();
}

bool GameManager::playGame() {
    while (!board.isGameOver() && !gameEnded) {
        clearScreen();
        displayGameInfo();
        
        Color current = board.getCurrentPlayer();
        std::string currentName = (current == Color::BLACK) ? player1Name : player2Name;
        
        auto validMoves = board.getValidMoves(current);
        if (validMoves.empty()) {
            InputHandler::displayInfoMessage("No valid moves for " + currentName + "! Turn passes.");
            board.switchPlayer();
            waitForEnter();
            continue;
        }
        
     
        if (handleBotTurn(current)) {
            if (checkAndHandleGameOver()) {
                return true;
            }
            waitForEnter();
            continue;
        }
        
      
        std::cout << "\nTurn: " << currentName 
                  << " (" << (current == Color::BLACK ? "Black" : "White") << ")\n";
        std::cout << "Enter position (e.g., e3) or 'q' to quit: ";
        
        std::string input;
        std::cin >> input;
        
        if (!processPlayerMove(current, input)) {
            if (gameEnded) {
                return false;
            }
            waitForEnter();
            continue;
        }
        
        if (checkAndHandleGameOver()) {
            return true;
        }
    }
    
    if (!gameEnded) {
        return endGame();
    }
    return false;
}