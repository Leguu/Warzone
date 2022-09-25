cmake_minimum_required(VERSION 3.23)
project(Warzone)

set(CMAKE_CXX_STANDARD 23)

add_executable(Warzone src/main.cpp src/Player/Player.cpp src/Player/Player.h src/Map/Map.cpp src/Map/Map.h src/Orders/Order.cpp src/Orders/Order.h src/Cards/Card.cpp src/Cards/Card.h src/GameEngine/GameEngine.cpp src/GameEngine/GameEngine.h src/Utils/Utils.cpp src/Utils/Utils.h src/Cards/CardsTester.cpp src/Cards/CardsTester.h src/Cards/CardsDriver.cpp src/Cards/CardsDriver.h)
