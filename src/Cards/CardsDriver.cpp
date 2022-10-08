//
// Created by danie on 2022-10-08.
//
#include "CardsDriver.h"
#include <iostream>
#include "Card.h"
#include "../GameEngine/GameEngine.h"
using std::cout;
using std::endl;

int main() {
    CardsDriver::testCards();
}

/**
 * Verify that we can create a valid deck, draw all its cards, runGameLoop them, and everything is functional
 */
void CardsDriver::testCards() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new NegotiateCard(), new BlockadeCard()};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);

    auto playerOne = new Player("Bob");
    auto playerTwo = new Player("CoolerBob");

    auto territoryOne = new Territory("potato");
    auto territoryTwo = new Territory("coolerPotato");
    auto territoryThree = new Territory("coolestPotato");

    territoryOne->setOwner(playerOne);
    territoryThree->setOwner(playerOne);
    territoryTwo->setOwner(playerTwo);

    territoryOne->addAdjacent(territoryTwo);
    territoryOne->addAdjacent(territoryThree);
    territoryTwo->addAdjacent(territoryOne);
    territoryTwo->addAdjacent(territoryThree);
    territoryThree->addAdjacent(territoryTwo);
    territoryThree->addAdjacent(territoryOne);

    territoryOne->setArmies(20);
    territoryThree->setArmies(15);

    ge->players.push_back(playerOne);
    ge->players.push_back(playerTwo);

    ge->map->addContinent(new Continent("potato", 5));
    ge->map->addTerritoryToMap(territoryOne);
    ge->map->addTerritoryToMap(territoryTwo);
    ge->map->addTerritoryToMap(territoryThree);

    Utils::assertCondition(playerOne->hand->cards.empty(), "player has no cards");

    while (ge->deck->getCardsSize() != 0) {
        playerOne->hand->draw();
    }

    Utils::assertCondition(playerOne->hand->cards.size() == cards.size(), "player drew all the cards in the deck");

    while (!playerOne->hand->cards.empty()) {
        auto cardName = playerOne->hand->cards[0]->name;
        cout << "Playing " << cardName << endl;
        // We use debugPlay instead of play so that we don't have to put a bunch of user input
        playerOne->hand->debugPlay(cardName);
    }

    Utils::assertCondition(playerOne->hand->cards.empty(), "all the cards were removed from the player's hand");
    Utils::assertCondition(ge->deck->getCardsSize() == cards.size(), "all the cards were put in the deck");

    cout << "All tests passed!" << endl;

    delete ge;
}
