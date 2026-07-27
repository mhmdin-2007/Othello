#include "../include/board.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

constexpr int Board::BOARD_SIZE;

Board::Board() {
    initialize();
}

bool Board::isOnBoard(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

void Board::initialize() {
    for (auto& row : grid) {
        row.fill(Color::EMPTY);
    }
    
    grid[INITIAL_ROW][INITIAL_COL] = Color::BLACK;
    grid[INITIAL_ROW][INITIAL_COL + 1] = Color::WHITE;
    grid[INITIAL_ROW + 1][INITIAL_COL] = Color::WHITE;
    grid[INITIAL_ROW + 1][INITIAL_COL + 1] = Color::BLACK;
    
    currentPlayer = Color::BLACK;
    updateCounts();
}

void Board::updateCounts() {
    blackCount = 0;
    whiteCount = 0;
    for (const auto& row : grid) {
        for (Color cell : row) {
            if (cell == Color::BLACK) blackCount++;
            else if (cell == Color::WHITE) whiteCount++;
        }
    }
}

void Board::display() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            char ch = static_cast<char>(grid[i][j]);
            std::cout << ch << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nBlack: " << blackCount << "  |  White: " << whiteCount << std::endl;
    std::cout << "Turn: " << (currentPlayer == Color::BLACK ? "Black" : "White") << std::endl;
}

Color Board::getCell(int row, int col) const {
    return isOnBoard(row, col) ? grid[row][col] : Color::EMPTY;
}

void Board::setCell(int row, int col, Color color) {
    if (isOnBoard(row, col)) {
        grid[row][col] = color;
    }
}

Color Board::getCurrentPlayer() const {
    return currentPlayer;
}

void Board::setCurrentPlayer(Color player) {
    currentPlayer = player;
}

void Board::switchPlayer() {
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

int Board::getBlackCount() const{
    return blackCount;
}

int Board::getWhiteCount() const {
    return whiteCount;
}

bool Board::checkDirection(int row, int col, int dRow, int dCol, Color color) const {
    Color opponent = (color == Color::BLACK) ? Color::WHITE : Color::BLACK;
    int r = row + dRow;
    int c = col + dCol;
    bool hasOpponent = false;

    while(isOnBoard(r, c) && grid[r][c] == opponent) {
        hasOpponent = true;
        r += dRow;
        c += dCol;
    }

    if(isOnBoard(r, c) && grid[r][c] == color){
        return true;
    }
    return false;
}

bool Board::isValidMove(int row, int col, Color color) const {
    if (!isOnBoard(row, col) || grid[row][col] != Color::EMPTY) {
        return false;
    }

    constexpr int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions){
        if (checkDirection(row, col, dir[0], dir[1], color)){
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> Board::getValidMoves(Color color) const {
    std::vector<std::pair<int, int>> moves;
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(isValidMove(i, j, color)) {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

bool Board::hasValidMoves(Color color) const {
    return !getValidMoves(color).empty();
}

void Board::makeMove(int row, int col, Color color) {
    if (!isValidMove(row, col, color)) {
        return;
    }

    grid[row][col] = color;
    
    constexpr int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : directions) {
        if (checkDirection(row, col, dir[0], dir[1], color)) {
            int r = row + dir[0];
            int c = col + dir[1];
            Color opponent = (color == Color::BLACK) ? Color::WHITE : Color::BLACK;
            while (isOnBoard(r, c) && grid[r][c] == opponent) {
                grid[r][c] = color;
                r += dir[0];
                c += dir[1];
            }
        }
    }
    updateCounts();
}

bool Board::isGameOver() const {
    return !hasValidMoves(Color::BLACK) && !hasValidMoves(Color::WHITE);
}

Color Board::getWinner() const {
    if(blackCount < whiteCount) {
        return Color::BLACK;
    }else if (whiteCount < blackCount){
        return Color::WHITE;
    }else{
        return Color::DRAW;
    }
}

std::string Board::serialize() const {
    std::stringstream ss;
    for(const auto& row : grid){
        for (Color cell : row){
            ss << static_cast<char>(cell);
        }
    }
    ss << "," << static_cast<char>(currentPlayer);
    return ss.str();
}

void Board::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string boardStr, playerStr;
    std::getline(ss, boardStr, ',');
    std::getline(ss, playerStr);

    int idx = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            grid[i][j] = static_cast<Color>(boardStr[idx++]);
        }
    }
    currentPlayer = static_cast<Color>(playerStr[0]);
    updateCounts();
}