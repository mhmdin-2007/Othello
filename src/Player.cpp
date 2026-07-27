#include "../include/Player.hpp"
#include <sstream>

Player::Player() : id(0), bestScore(0) {}

Player::Player(int id, const std::string& username, const std::string& password, int bestScore)
    : id(id), username(username), password(password), bestScore(bestScore) {}

int Player::getId() const {
    return id;
}

std::string Player::getUsername() const {
    return username;
}

std::string Player::getPassword() const {
    return password;
}

int Player::getBestScore() const {
    return bestScore;
}

void Player::setBestScore(int score) {
    bestScore = score;
}

std::string Player::toString() const {
    std::stringstream ss;
    ss << id << "," << username << "," << password << "," << bestScore;
    return ss.str();
}