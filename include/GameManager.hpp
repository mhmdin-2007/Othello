#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <string>
#include "board.hpp"
#include "bot.hpp"

class GameManager {
private:
    Board board;
    std::string player1Name;
    std::string player2Name;
    bool isBotGame;
    Bot bot;
    bool gameEnded;
    bool isGameSaved;

    void clearScreen() const;
    void waitForEnter() const;
    void displayGameInfo() const;

    bool processPlayerMove(Color current, const std::string& input);
    bool handleBotTurn(Color current);                                
    bool endGame();
    
    bool checkAndHandleGameOver();

public:
    GameManager();
    
    [[nodiscard]] bool startGame(const std::string& p1, const std::string& p2, 
                                 bool botMode = false, int botLevel = 1);
    [[nodiscard]] bool playGame();
};

#endif