cmake_minimum_required(VERSION 3.10)
project(Othello)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs")

include_directories(include)

add_executable(othello
    src/main.cpp
    src/Board.cpp
    src/Player.cpp
    src/Bot.cpp
    src/FileManager.cpp
    src/GameManager.cpp
    src/Menu.cpp
    src/InputHandler.cpp
)

target_link_libraries(othello stdc++fs)