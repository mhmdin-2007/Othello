#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <string>
#include <utility>
#include <vector>

class InputHandler {
public:
    [[nodiscard]] static bool parsePosition(const std::string& input, int& row, int& col);
    [[nodiscard]] static bool isValidPosition(int row, int col);
    [[nodiscard]] static bool isValidMenuChoice(int choice, int min, int max);
    
    [[nodiscard]] static bool isValidUsername(const std::string& username);
    
    [[nodiscard]] static bool isValidPassword(const std::string& password);
    
    [[nodiscard]] static std::string sanitizeInput(std::string input);
    
    static void displayValidMoves(const std::vector<std::pair<int,int>>& moves);
    static void displayErrorMessage(const std::string& message);
    static void displaySuccessMessage(const std::string& message);
    static void displayInfoMessage(const std::string& message);
};

#endif 