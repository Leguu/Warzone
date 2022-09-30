//
// Created by danie on 2022-09-24.
//

#include "CardsDriver.h"
#include "./Card.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
#include "../Utils/Utils.h"
#include <iostream>


int main() {
/* ============================================================================================
 * Testing Cards Class Elements
 * ============================================================================================
 * */
    auto divider = "================================================================";
    /* ---------------------------- Testing All Methods ---------------------------- */
    int input = 0;
    while (true) {
        input = Utils::getInputInt(
                "Please input:\n1 To call testCards function \n2 To test all constructors\n3 To test all Play functions\n4 To test all Cards interaction functions\n5 To test all toString functions\n-1 to exit\n");
        if (input == 1) {
            std::cout << divider << std::endl;
            std::cout << "Cards Driver : TestCards method" << std::endl;
            std::cout << divider << std::endl;
            testCards();
        } else if (input == 2) {
            std::cout << divider << std::endl;
            std::cout << "Testing Cards.cpp Constructors" << std::endl;
            std::cout << divider << std::endl;
            CardsTester::testBombCardConstructor();
            CardsTester::testBlockadeCardConstructor();
            CardsTester::testAirliftCardConstructor();
            CardsTester::testNegotiateCardConstructor();
            CardsTester::testDeckConstructor();
            CardsTester::testHandConstructor();
        } else if (input == 3) {
            std::cout << divider << std::endl;
            std::cout << "Testing Cards.cpp Play functions" << std::endl;
            std::cout << divider << std::endl;
            CardsTester::testBombCardPlay();
            CardsTester::testBlockadeCardPlay();
            CardsTester::testAirliftCardPlay();
            CardsTester::testNegotiateCardPlay();
        } else if (input == 4) {
            std::cout << divider << std::endl;
            std::cout << "Testing Cards.cpp Card Interaction functions" << std::endl;
            std::cout << divider << std::endl;
            CardsTester::testHandRemove();
            CardsTester::testDeckDraw();
            CardsTester::testDeckPut();
        } else if (input == 5) {
            std::cout << divider << std::endl;
            std::cout << "Testing Cards.cpp toString functions" << std::endl;
            std::cout << divider << std::endl;
            CardsTester::testDeckToString();
            CardsTester::testHandToString();
            CardsTester::testBombCardToString();
            CardsTester::testBlockadeCardToString();
            CardsTester::testAirliftCardToString();
            CardsTester::testNegotiateCardToString();
            CardsTester::testListHand();
        } else if (input == -1) {
            exit(-1);
        }
    }


}

/**
 * Verify that we can create a valid deck, draw all its cards, play them, and everything is functional
 */
void testCards() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new NegotiateCard, new BlockadeCard, new BlockadeCard};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto playerOne = new Player("Bob");
    auto playerTwo = new Player("CoolerBob");
    auto territoryOne = new Territory("potato", "potato");
    auto territoryTwo = new Territory("coolerPotato", "potato");
    auto territoryThree = new Territory("coolestPotato", "potato");
    playerOne->ownedTerritories.push_back(territoryOne);
    playerOne->ownedTerritories.push_back(territoryThree);
    playerTwo->ownedTerritories.push_back(territoryTwo);
    territoryOne->setOwner(playerOne);
    territoryOne->setArmies(20);
    territoryThree->setOwner(playerOne);
    territoryThree->setArmies(15);
    territoryTwo->setOwner(playerTwo);
    ge->players.push_back(playerOne);
    ge->players.push_back(playerTwo);
    ge->map->addContinent(new Continent("potato", 5));
    ge->map->addTerritoryToMap(territoryOne);
    ge->map->addTerritoryToMap(territoryTwo);
    ge->map->addTerritoryToMap(territoryThree);
    ge->map->addEdge(territoryOne, territoryTwo);
    ge->map->addEdge(territoryOne, territoryThree);
    ge->map->addEdge(territoryThree, territoryTwo);
    playerOne->hand = new Hand();
    while (ge->deck->getCardsSize() != 0) {
        playerOne->hand->draw();
    }
    while (!playerOne->hand->cards.empty()) {
        playerOne->play(playerOne->hand->cards[0]->name);
    }
    std::cout << "Content of Deck after every card has been played: " << *ge->deck << std::endl;
    std::cout << "Content of the hand after card has been played: " << *playerOne->hand << std::endl;
}

/**
 * Test the BombCard constructor
 */
void CardsTester::testBombCardConstructor() {
    auto card = new BombCard();
    std::cout << "Bomb Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

/**
 * Test the BombCard play function
 */
void CardsTester::testBombCardPlay() {
    auto ge = new GameEngine("../assets/Moon.map");
    auto playerOne = new Player("Bob");
    auto playerTwo = new Player("CoolerBob");
    auto territoryOne = new Territory("potato", "potato");
    auto territoryTwo = new Territory("coolerPotato", "potato");
    playerOne->ownedTerritories.push_back(territoryOne);
    playerTwo->ownedTerritories.push_back(territoryTwo);
    territoryOne->setOwner(playerOne);
    territoryTwo->setOwner(playerTwo);
    ge->deck = new Deck({new BombCard()});
    ge->players.push_back(playerOne);
    ge->players.push_back(playerTwo);
    ge->map->addContinent(new Continent("potato", 5));
    ge->map->addTerritoryToMap(territoryOne);
    ge->map->addTerritoryToMap(territoryTwo);
    ge->map->addEdge(territoryOne, territoryTwo);
    auto card = playerOne->hand->draw();
    auto orderSizeBeforePlay = playerOne->orders->getOrdersSize();
    playerOne->play(card->name);
    auto orderSizeAfterPlay = playerOne->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testBombCardPlay");
}

/**
 * Test the BlockadeCard constructor
 */
void CardsTester::testBlockadeCardConstructor() {
    auto card = new BlockadeCard();
    std::cout << "Blockade Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the Blockade Card play function
 */
void CardsTester::testBlockadeCardPlay() {
    auto ge = new GameEngine("../assets/Moon.map");
    auto playerOne = new Player("Bob");
    auto territoryOne = new Territory("potato", "potato");
    playerOne->ownedTerritories.push_back(territoryOne);
    territoryOne->setOwner(playerOne);
    ge->deck = new Deck({new BlockadeCard()});
    ge->players.push_back(playerOne);
    ge->map->addTerritoryToMap(territoryOne);
    auto card = playerOne->hand->draw();
    auto orderSizeBeforePlay = playerOne->orders->getOrdersSize();
    playerOne->play(card->name);
    auto orderSizeAfterPlay = playerOne->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testBlockadeCardPlay");

}

/**
 * Test the AirliftCard constructor
 */
void CardsTester::testAirliftCardConstructor() {
    auto card = new AirliftCard();
    std::cout << "Airlift Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the AirliftCard play function
 */
void CardsTester::testAirliftCardPlay() {
    auto ge = new GameEngine("../assets/Moon.map");
    auto playerOne = new Player("Bob");
    auto territoryOne = new Territory("potato", "potato");
    auto territoryTwo = new Territory("coolerPotato", "potato");
    auto territoryThree = new Territory("uncoolPotato", "potato");
    playerOne->ownedTerritories.push_back(territoryOne);
    territoryOne->setArmies(10);
    playerOne->ownedTerritories.push_back(territoryTwo);
    territoryOne->setOwner(playerOne);
    territoryTwo->setOwner(playerOne);
    ge->deck = new Deck({new AirliftCard()});
    ge->players.push_back(playerOne);
    ge->map->addContinent(new Continent("potato", 5));
    ge->map->addTerritoryToMap(territoryOne);
    ge->map->addTerritoryToMap(territoryTwo);
    ge->map->addTerritoryToMap(territoryThree);
    auto card = playerOne->hand->draw();
    auto orderSizeBeforePlay = playerOne->orders->getOrdersSize();
    playerOne->play(card->name);
    auto orderSizeAfterPlay = playerOne->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testAirliftCardPlay");
}

/**
 * Test the NegotiateCard constructor
 */
void CardsTester::testNegotiateCardConstructor() {
    auto card = new NegotiateCard();
    std::cout << "Negotiate Card built successfully : name : " << card->name << ", desc: " << card->description
              << std::endl;
}

/**
 * Test the NegotiateCard play function
 */
void CardsTester::testNegotiateCardPlay() {
    auto ge = new GameEngine("../assets/Moon.map");
    auto playerOne = new Player("Bob");
    auto playerTwo = new Player("CoolerBob");
    auto playerThree = new Player("MostCoolBob");
    auto playerFour = new Player("UncoolBob");
    ge->deck = new Deck({new NegotiateCard()});
    ge->players.push_back(playerOne);
    ge->players.push_back(playerTwo);
    ge->players.push_back(playerThree);
    ge->players.push_back(playerFour);
    auto card = playerOne->hand->draw();
    auto orderSizeBeforePlay = playerOne->orders->getOrdersSize();
    playerOne->play(card->name);
    auto orderSizeAfterPlay = playerOne->orders->getOrdersSize();
    Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testAirliftCardPlay");

}

/**
 * Test the Hand constructor
 */
void CardsTester::testHandConstructor() {
    auto hand = new Hand();
    std::cout << "Hand successfully built, should be empty. Number of cards held in hand: " << hand->cards.size()
              << std::endl;
}

/**
 * Test the Hand Listhand function
 */
void CardsTester::testListHand() {
    auto hand = new Hand();
    hand->listHand();
    std::cout << "Finished printing Hand\n" << std::endl;
}

/**
 * Test the Deck constructor
 */
void CardsTester::testDeckConstructor() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto deck = new Deck(cards);
    std::cout << "Deck successfully built, should contain a n>0 number of cards: " << deck->getCardsSize() << std::endl;
}

/**
 * Test the Deck draw function
 */
void CardsTester::testDeckDraw() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto ge = new GameEngine("../assets/Moon.map");
    ge->deck = new Deck(cards);
    auto initialDeckSize = ge->deck->getCardsSize();
    ge->deck->draw();
    auto drawnDeckSize = ge->deck->getCardsSize();
    Utils::assert(initialDeckSize == drawnDeckSize + 1, "testDeckDraw");
}

/**
 * Test the Deck put function
 */
void CardsTester::testDeckPut() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto deck = new Deck(cards);
    auto card = new BlockadeCard();
    auto initialDeckSize = deck->getCardsSize();
    deck->put(card);
    auto addedDeckSize = deck->getCardsSize();
    Utils::assert(initialDeckSize == addedDeckSize - 1, "testDeckPut");
}

/**
 * Test the Deck toString function
 */
void CardsTester::testDeckToString() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto deck = new Deck(cards);
    std::cout << *deck << std::endl;
    std::cout << "Finished printing Deck\n" << std::endl;
}

/**
 * Test the Hand toString function
 */
void CardsTester::testHandToString() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto deck = new Deck(cards);
    auto hand = new Hand();
    auto ge = new GameEngine("assets/Moon.map");
    ge->deck = deck;
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    std::cout << *hand << std::endl;
    std::cout << "Finished printing Hand\n" << std::endl;
}

/**
 * Test the BombCard toString function
 */
void CardsTester::testBombCardToString() {
    auto card = new BombCard();
    std::cout << *card;
    std::cout << "\nFinished printing BombCard\n" << std::endl;
}

/**
 * Test the BlockadeCard toString function
 */
void CardsTester::testBlockadeCardToString() {
    auto card = new BlockadeCard();
    std::cout << *card;
    std::cout << "\nFinished printing BlockadeCard\n" << std::endl;
}

/**
 * Test the AirliftCard toString function
 */
void CardsTester::testAirliftCardToString() {
    auto card = new AirliftCard();
    std::cout << *card;
    std::cout << "\nFinished printing AirliftCard\n" << std::endl;

}

/**
 * Test the NegotiateCard toString function
 */
void CardsTester::testNegotiateCardToString() {
    auto card = new NegotiateCard();
    std::cout << *card;
    std::cout << "\nFinished printing NegotiateCard\n" << std::endl;
}

/**
 * Test the Hand remove function
 */
void CardsTester::testHandRemove() {
    std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                                 new BlockadeCard};
    auto deck = new Deck(cards);
    auto hand = new Hand();
    auto ge = new GameEngine("assets/Moon.map");
    ge->deck = deck;
    while (deck->getCardsSize() != 0) {
        hand->draw();
    }
    auto addedHandSize = hand->cards.size();
    hand->remove(1);
    auto removedHandSize = hand->cards.size();
    Utils::assert(addedHandSize == removedHandSize + 1, "testHandRemove");
}


