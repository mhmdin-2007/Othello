#include "../include/InputHandler.hpp"
#include <iostream>
#include <cctype>
#include <algorithm>

bool InputHandler::parsePosition(const std::string& input, int& row, int& col) {
    if (input.length() < 2) {
        return false;
    }
    
    std::string clean = sanitizeInput(input);
    if (clean.length() < 2) {
        return false;
    }
    
    std::transform(clean.begin(), clean.end(), clean.begin(), ::tolower);
    

    char colChar = clean[0];
    if (colChar < 'a' || colChar > 'h') {
        return false;
    }
    col = colChar - 'a';
    if (!std::isdigit(clean[1])) {
        return false;
    }
    row = clean[1] - '1';
    return isValidPosition(row, col);
}

bool InputHandler::isValidPosition(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool InputHandler::isValidMenuChoice(int choice, int min, int max) {
    return choice >= min && choice <= max;
}

bool InputHandler::isValidUsername(const std::string& username) {
    if (username.empty() || username.length() < 3) {
        return false;
    }

    for (char c : username) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool InputHandler::isValidPassword(const std::string& password) {
    return password.length() >= 3;
}

std::string InputHandler::sanitizeInput(std::string input) {
    // حذف فاصله‌های اضافی از ابتدا و انتها
    input.erase(0, input.find_first_not_of(" \t\n\r"));
    input.erase(input.find_last_not_of(" \t\n\r") + 1);
    return input;
}

void InputHandler::displayValidMoves(const std::vector<std::pair<int,int>>& moves) {
    if (moves.empty()) {
        std::cout << "No valid moves available!\n";
        return;
    }
    
    std::cout << "Valid moves: ";
    for (const auto& move : moves) {
        std::cout << char(move.second + 'a') << (move.first + 1) << " ";
    }
    std::cout << std::endl;
}

void InputHandler::displayErrorMessage(const std::string& message) {
    std::cerr << "❌ Error: " << message << std::endl;
}

void InputHandler::displaySuccessMessage(const std::string& message) {
    std::cout << "✅ " << message << std::endl;
}

void InputHandler::displayInfoMessage(const std::string& message) {
    std::cout << "ℹ️  " << message << std::endl;
}