#include "../include/Bot.hpp"
#include<random>

Bot::Bot(Difficulty level)
    : level(level), rng(std::random_device{}()) {}

void Bot::setLevel(Difficulty lvl) {
    level = lvl;
}

std::pair<int, int> Bot::getMove(const Board& board, Color botColor) const {
    std::vector<std::pair<int, int>> moves = board.getValidMoves(botColor);

    if (moves.empty()) {
        return {-1, -1};
    }

    switch (level) {
        case Difficulty::EASY:
            return moves[rng() % moves.size()];
            
        case Difficulty::MEDIUM:
            return getBestCaptureMove(board, botColor);            
        case Difficulty::HARD:
            return getHardMove(board, botColor);
            
        default:
            return moves[0];
    }
}

//استراتژی متوسط - گرفتن بیشترین مهره
std::pair<int,int> Bot::getBestCaptureMove(const Board& board, Color color) const {
    std::vector<std::pair<int,int>> moves = board.getValidMoves(color);
    std::pair<int,int> bestMove = moves[0];
    int maxCaptures = -1;
    
    for (auto move : moves) {
        Board tempBoard = board;
        tempBoard.makeMove(move.first, move.second, color);
        
        int beforeCount = (color == Color::BLACK) ? 
                          board.getBlackCount() : board.getWhiteCount();
        int afterCount = (color == Color::BLACK) ? 
                         tempBoard.getBlackCount() : tempBoard.getWhiteCount();
        int captures = afterCount - beforeCount;
        
        if (captures > maxCaptures) {
            maxCaptures = captures;
            bestMove = move;
        }
    }
    return bestMove;
}

//استراتژی سخت - ازیابی عمیق تر
std::pair<int,int> Bot::getHardMove(const Board& board, Color color) const {
    std::vector<std::pair<int,int>> moves = board.getValidMoves(color);
    std::pair<int,int> bestMove = moves[0];
    int bestScore = -1000;
    
    for (auto move : moves) {
        Board tempBoard = board;
        tempBoard.makeMove(move.first, move.second, color);
        
        int score = evaluateBoard(tempBoard, color);
        
        Color opponent = (color == Color::BLACK) ? Color::WHITE : Color::BLACK;
        std::vector<std::pair<int,int>> opponentMoves = tempBoard.getValidMoves(opponent);
        
        for (auto oppMove : opponentMoves) {
            Board tempBoard2 = tempBoard;
            tempBoard2.makeMove(oppMove.first, oppMove.second, opponent);
            score -= evaluateBoard(tempBoard2, color) * 0.5;
        }
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}


//ارزیابی صفحه
int Bot::evaluateBoard(const Board& board, Color color) const {
    int score = 0;
    
    int myCount = (color == Color::BLACK) ? 
                  board.getBlackCount() : board.getWhiteCount();
    int oppCount = (color == Color::BLACK) ? 
                   board.getWhiteCount() : board.getBlackCount();
    score += (myCount - oppCount) * 10;
    
    const int corners[4][2] = {{0,0}, {0,7}, {7,0}, {7,7}};
    for (const auto& corner : corners) {
        if (board.getCell(corner[0], corner[1]) == color) {
            score += 50;  
        } else if (board.getCell(corner[0], corner[1]) != Color::EMPTY) {
            score -= 30;
        }
    }
    
    return score;
}