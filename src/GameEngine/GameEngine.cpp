#include <iostream>
#include "GameEngine.h"

GameEngine *GameEngine::_instance = nullptr;

GameEngine::GameEngine(const std::string &mp) {
    map = MapLoader::importMap(mp);
    _instance = this;
}

/// @returns Game Over
bool GameEngine::executeOrders() {
    std::cout << std::endl << "Executing orders..." << std::endl;
    for (auto &player: this->players) {
        while (auto order = player->orders->pop()) {
            try {
                order->execute();
                std::cout << *order << std::endl;
                if (map->allContinentsOwned()) {
                    return true;
                }
            } catch (InvalidOrderException &e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    std::cout << "... All orders have been executed" << std::endl << std::endl;
    return false;
}


//Player *GameEngine::findPlayerByName(std::string name) {
//    for (auto &eachPlayer: this->players) {
//        if (eachPlayer->name == name) {
//            return std::make_unique<Player>(eachPlayer);
//        }
//    }
//}

/**
 * Draw a card from the deck
 * @return The card that has been drawn
 */
Card* Deck::draw() {
    auto* card = this->cardCollection.back();
    //not sure if this will destroy card's reference
    this->cardCollection.pop_back();
    return card;
}

void GameEngine::play() {
    initialisePlayers();

    while (true) {
        assignReinforcements();

        issueOrders();

        auto gameOver = executeOrders();
        // If the game is over
        if (gameOver) {
            break;
        }
    }

    std::cout << "Game is over!" << std::endl;
}

/**
 * List the contents of the deck
 */
void GameEngine::listDeck() {
    std::cout << this->deck << std::endl;
}
