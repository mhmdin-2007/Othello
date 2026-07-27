#ifndef BOT_HPP
#define BOT_HPP

#include<utility>
#include<random>
#include "Board.hpp"
#include "Enums.hpp"

class Bot 
{
private:
    Difficulty level;
    mutable std::mt19937 rng; //تولید کننده اعداد تصادفی مدرن

    [[nodiscard]] std::pair<int, int> getBestCaptureMove(const Board& board, Color color) const;
    [[nodiscard]] std::pair<int, int> getHardMove(const Board& board, Color color) const;
    [[nodiscard]] int evaluateBoard(const Board& board, Color color) const;

public:

    explicit Bot(Difficulty level = Difficulty::EASY);

    void setLevel(Difficulty lvl);

    [[nodiscard]] std::pair<int, int> getMove(const Board& board, Color botColor) const;

};

#endif