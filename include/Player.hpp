#ifndef PLAYER_HPP
#define PLAYER_HPP

#include<string>

class Player {
private:
    int id;
    std::string username;
    std::string password;
    int bestScore;

public:
    Player();
    Player(int id, const std::string& username, const std::string& password, int bestScore = 0);

    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getUsername() const;
    [[nodiscard]] std::string getPassword() const;
    [[nodiscard]] int getBestScore() const;

    void setBestScore(int score);

    [[nodiscard]] std::string toString() const;
};

#endif