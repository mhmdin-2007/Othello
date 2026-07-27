#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <vector>
#include <string>
#include <utility>
#include "Enums.hpp"

class Board {
private:
    static constexpr int BOARD_SIZE = 8;
    static constexpr int INITIAL_ROW = 3;
    static constexpr int INITIAL_COL = 3;
    
    std::array<std::array<Color, BOARD_SIZE>, BOARD_SIZE> grid;
    Color currentPlayer;
    int blackCount, whiteCount;

    [[nodiscard]] bool isOnBoard(int row, int col) const;
    void updateCounts();
    [[nodiscard]] bool checkDirection(int row, int col, int dRow, int dCol, Color color) const;

public:
    Board();
    void initialize();
    void display() const;
    
    [[nodiscard]] Color getCell(int row, int col) const;
    void setCell(int row, int col, Color color);
    
    [[nodiscard]] Color getCurrentPlayer() const;
    void setCurrentPlayer(Color player);
    void switchPlayer();
    
    [[nodiscard]] int getBlackCount() const;
    [[nodiscard]] int getWhiteCount() const;
    
    [[nodiscard]] bool isValidMove(int row, int col, Color color) const;
    [[nodiscard]] std::vector<std::pair<int,int>> getValidMoves(Color color) const;
    [[nodiscard]] bool hasValidMoves(Color color) const;
    
    void makeMove(int row, int col, Color color);
    
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] Color getWinner() const;
    
    [[nodiscard]] std::string serialize() const;
    void deserialize(const std::string& data);
    
    Board(const Board& other) = default;
    Board& operator=(const Board& other) = default;
};

#endif