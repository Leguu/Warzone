//
// Created by danie on 2022-09-24.
//

#include "./Card.h"
#include "../GameEngine/GameEngine.h"
#include <iostream>

/**
 * Verify that we can create a valid deck, draw all its cards, runGameLoop them, and everything is functional
 */
void testCards() {
  std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                               new NegotiateCard, new BlockadeCard, new BlockadeCard};
  auto ge = new GameEngine("../assets/Moon.map");
  ge->deck = new Deck(cards);
  auto playerOne = new Player("Bob");
  auto playerTwo = new Player("CoolerBob");
  auto territoryOne = new Territory("potato");
  auto territoryTwo = new Territory("coolerPotato");
  auto territoryThree = new Territory("coolestPotato");
  playerOne->ownedTerritories.push_back(territoryOne);
  playerOne->ownedTerritories.push_back(territoryThree);
  playerTwo->ownedTerritories.push_back(territoryTwo);
  territoryOne->addAdjacent(territoryTwo);
  territoryOne->addAdjacent(territoryThree);
  territoryTwo->addAdjacent(territoryOne);
  territoryTwo->addAdjacent(territoryThree);
  territoryThree->addAdjacent(territoryTwo);
  territoryThree->addAdjacent(territoryOne);
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
void testBombCardConstructor() {
  auto card = new BombCard();
  std::cout << "Bomb Card built successfully : name : " << card->name << ", desc: " << card->description << std::endl;
}

/**
 * Test the BombCard runGameLoop function
 */
void testBombCardPlay() {
  auto ge = new GameEngine("../assets/Moon.map");
  auto playerOne = new Player("Bob");
  auto playerTwo = new Player("CoolerBob");
  auto territoryOne = new Territory("potato");
  auto territoryTwo = new Territory("coolerPotato");
  playerOne->ownedTerritories.push_back(territoryOne);
  playerTwo->ownedTerritories.push_back(territoryTwo);
  territoryOne->setOwner(playerOne);
  territoryTwo->setOwner(playerTwo);
  ge->deck = new Deck({new BombCard()});
  ge->players.push_back(playerOne);
  ge->players.push_back(playerTwo);
  ge->map->addContinent(new Continent("potatoLand", 5));
  ge->map->addTerritoryToMap(territoryOne);
  ge->map->addTerritoryToMap(territoryTwo);
  territoryOne->addAdjacent(territoryTwo);
  territoryTwo->addAdjacent(territoryOne);
  auto card = playerOne->hand->draw();
  auto orderSizeBeforePlay = playerOne->orders->getOrdersSize();
  playerOne->play(card->name);
  auto orderSizeAfterPlay = playerOne->orders->getOrdersSize();
  Utils::assert(orderSizeBeforePlay + 1 == orderSizeAfterPlay, "testBombCardPlay");
}

/**
 * Test the BlockadeCard constructor
 */
void testBlockadeCardConstructor() {
  auto card = new BlockadeCard();
  std::cout << "Blockade Card built successfully : name : " << card->name << ", desc: " << card->description
            << std::endl;
}

/**
 * Test the Blockade Card runGameLoop function
 */
void testBlockadeCardPlay() {
  auto ge = new GameEngine("../assets/Moon.map");
  auto playerOne = new Player("Bob");
  auto territoryOne = new Territory("potato");
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
void testAirliftCardConstructor() {
  auto card = new AirliftCard();
  std::cout << "Airlift Card built successfully : name : " << card->name << ", desc: " << card->description
            << std::endl;
}

/**
 * Test the AirliftCard runGameLoop function
 */
void testAirliftCardPlay() {
  auto ge = new GameEngine("../assets/Moon.map");
  auto playerOne = new Player("Bob");
  auto territoryOne = new Territory("potato");
  auto territoryTwo = new Territory("coolerPotato");
  auto territoryThree = new Territory("uncoolPotato");
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
void testNegotiateCardConstructor() {
  auto card = new NegotiateCard();
  std::cout << "Negotiate Card built successfully : name : " << card->name << ", desc: " << card->description
            << std::endl;
}

/**
 * Test the NegotiateCard runGameLoop function
 */
void testNegotiateCardPlay() {
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
void testHandConstructor() {
  auto hand = new Hand();
  std::cout << "Hand successfully built, should be empty. Number of cards held in hand: " << hand->cards.size()
            << std::endl;
}

/**
 * Test the Hand Listhand function
 */
void testListHand() {
  auto hand = new Hand();
  hand->listHand();
  std::cout << "Finished printing Hand\n" << std::endl;
}

/**
 * Test the Deck constructor
 */
void testDeckConstructor() {
  std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                               new BlockadeCard};
  auto deck = new Deck(cards);
  std::cout << "Deck successfully built, should contain a n>0 number of cards: " << deck->getCardsSize() << std::endl;
}

/**
 * Test the Deck draw function
 */
void testDeckDraw() {
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
void testDeckPut() {
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
void testDeckToString() {
  std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                               new BlockadeCard};
  auto deck = new Deck(cards);
  std::cout << *deck << std::endl;
  std::cout << "Finished printing Deck\n" << std::endl;
}

/**
 * Test the Hand toString function
 */
void testHandToString() {
  std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                               new BlockadeCard};
  auto deck = new Deck(cards);
  auto hand = new Hand();
  auto ge = new GameEngine("../assets/Moon.map");
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
void testBombCardToString() {
  auto card = new BombCard();
  std::cout << *card;
  std::cout << "\nFinished printing BombCard\n" << std::endl;
}

/**
 * Test the BlockadeCard toString function
 */
void testBlockadeCardToString() {
  auto card = new BlockadeCard();
  std::cout << *card;
  std::cout << "\nFinished printing BlockadeCard\n" << std::endl;
}

/**
 * Test the AirliftCard toString function
 */
void testAirliftCardToString() {
  auto card = new AirliftCard();
  std::cout << *card;
  std::cout << "\nFinished printing AirliftCard\n" << std::endl;

}

/**
 * Test the NegotiateCard toString function
 */
void testNegotiateCardToString() {
  auto card = new NegotiateCard();
  std::cout << *card;
  std::cout << "\nFinished printing NegotiateCard\n" << std::endl;
}

/**
 * Test the Hand remove function
 */
void testHandRemove() {
  std::vector<Card *> cards = {new BombCard(), new AirliftCard(), new BombCard(), new NegotiateCard,
                               new BlockadeCard};
  auto deck = new Deck(cards);
  auto hand = new Hand();
  auto ge = new GameEngine("../assets/Moon.map");
  ge->deck = deck;
  while (deck->getCardsSize() != 0) {
    hand->draw();
  }
  auto addedHandSize = hand->cards.size();
  hand->remove(new BombCard());
  auto removedHandSize = hand->cards.size();
  Utils::assert(addedHandSize == removedHandSize + 1, "testHandRemove");
}

int main() {
/* ============================================================================================
 * Testing Cards Class Elements
 * ============================================================================================
 * */
  auto divider = "================================================================";
  /* ---------------------------- Testing All Methods ---------------------------- */
  int input = 0;
  while (true) {
    std::cout << "Please input:" << std::endl;
    std::cout << "1 to call testCards function" << std::endl;
    std::cout << "2 to test all constructors" << std::endl;
    std::cout << "3 to test all Play functions" << std::endl;
    std::cout << "4 to test all Cards interaction functions" << std::endl;
    std::cout << "5 to test all toString functions" << std::endl;
    std::cout << "6 to test asserts functions" << std::endl;
    std::cout << "-1 to exit" << std::endl;
    input = Utils::getInputInt("");
    if (input == 1) {
      std::cout << divider << std::endl;
      std::cout << "Cards Driver : TestCards method" << std::endl;
      std::cout << divider << std::endl;
      testCards();
    } else if (input == 2) {
      std::cout << divider << std::endl;
      std::cout << "Testing Cards.cpp Constructors" << std::endl;
      std::cout << divider << std::endl;
      testBombCardConstructor();
      testBlockadeCardConstructor();
      testAirliftCardConstructor();
      testNegotiateCardConstructor();
      testDeckConstructor();
      testHandConstructor();
    } else if (input == 3) {
      std::cout << divider << std::endl;
      std::cout << "Testing Cards.cpp Play functions" << std::endl;
      std::cout << divider << std::endl;
      testBombCardPlay();
      testBlockadeCardPlay();
      testAirliftCardPlay();
      testNegotiateCardPlay();
    } else if (input == 4) {
      std::cout << divider << std::endl;
      std::cout << "Testing Cards.cpp Card Interaction functions" << std::endl;
      std::cout << divider << std::endl;
      testHandRemove();
      testDeckDraw();
      testDeckPut();
    } else if (input == 5) {
      std::cout << divider << std::endl;
      std::cout << "Testing Cards.cpp toString functions" << std::endl;
      std::cout << divider << std::endl;
      testDeckToString();
      testHandToString();
      testBombCardToString();
      testBlockadeCardToString();
      testAirliftCardToString();
      testNegotiateCardToString();
      testListHand();
    } else if (input == 6) {
      std::cout << divider << std::endl;
      std::cout << "Testing the assert functions..." << std::endl;
      std::cout << divider << std::endl;

      Utils::assert(true, "This test should not fail.");
      Utils::assert(false, "This test should fail!");
    } else if (input == -1) {
      exit(-1);
    }
  }

}
