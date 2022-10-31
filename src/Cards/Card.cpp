#include <iostream>
#include "Card.h"
#include "../GameEngine/GameEngine.h"
#include <cstdlib>
using std::cout;
using std::endl;

/**
 * Function to print to console the content's of one's hand
 * @param os The OSStream we will be appending to
 * @param hand  The player hand we will be examining
 * @return A string containing the content's of the player's hand
 */
std::ostream &operator<<(std::ostream &os, const Hand &hand) {
  os << "You have " << hand.cards.size() << " cards in your hand." << endl;
  for (auto &card : hand.cards) {
    os << *card << endl;
  }
  return os;
}

/**
 * Function to print to console the content's of the deck
 * @param os The OSStream we will be appending to
 * @param deck  The deck we will be examining
 * @return A string containing the content's of the deck
 */
std::ostream &operator<<(std::ostream &os, const Deck &deck) {
  os << "You have " << deck.cards.size() << " cards in your deck." << endl;
  for (auto &card : deck.cards) {
    os << *card << endl;
  }
  return os;
}

/**
 * List the contents of the hand
 */
void Hand::listHand() const {
  cout << *this << endl;
}

/**
 * Remove a card from the hand
 * @param card The card that is removed
 */
void Hand::remove(Card *card) {
  int index = 0;
  for (auto cardIteration : this->cards) {
    if (cardIteration->name == card->name) {
      this->cards.erase(this->cards.begin() + index);
      return;
    }
    index++;
  }
}

/**
 * Add a card to the hand
 * @param card The card that is added
 */
void Hand::add(Card *card) {
  this->cards.push_back(card);
}

/**
 * Function to print to console the content's of the card
 * @param os The OSStream we will be appending to
 * @param card  The card we will be examining
 * @return A string containing the content's of the card
 */
std::ostream &operator<<(std::ostream &os, const Card &card) {
  os << card.name << " Card: " << card.description;
  return os;
}

/**
 * Return the aliases of a card
 * @return the aliases of a card
 */
std::vector<std::string> Card::getAliases() {
  return this->aliases;
}

/**
 * Destructor for the card
 */
Card::~Card() = default;

/**
 * Playing the bomb card
 * @param issuer The player playing the card
 */
bool BombCard::play(Player *issuer) const {
  auto ge = GameEngine::instance();
  auto adjacentEnemyTerritories = issuer->getAdjacentEnemyTerritories();
  while (true) {
    cout << "Adjacent enemy territories you can bomb: ";
    for (auto adjacentEnemyTerritory : adjacentEnemyTerritories) {
      cout << "* " << *adjacentEnemyTerritory << " ";
    }
    cout << endl;
    auto territory = ge->map->getInputTerritory("Please input the ID of the territory you will bomb.");
    if (territory->getOwner() == issuer) {
      cout << "Error: Cannot bomb your own territory!" << endl << endl;
      continue;
    }
    auto adjacentTerritory = std::find(adjacentEnemyTerritories.begin(), adjacentEnemyTerritories.end(), territory);
    if (adjacentTerritory == adjacentEnemyTerritories.end()) {
      cout << "Error: Can only bomb adjacent enemy territories!" << endl << endl;
      continue;
    }
    cout << endl;
    BombCard::execute(issuer, territory);
    return true;
  }
}

/**
 * add bomb card to the orders
 * @param issuer The player playing the card
 * @param territory The territory the card targets
 */
void BombCard::execute(Player *issuer, Territory *territory) {
  auto order = new BombOrder(issuer, territory);
  issuer->orders->push(order);
}

/**
 * Destructor for the Bomb Card
 */
BombCard::~BombCard() = default;

/**
 * Destructor for the Blockade Card
 */
BlockadeCard::~BlockadeCard() = default;

/**
 * Destructor for the Airlift Card
 */
AirliftCard::~AirliftCard() = default;

/**
 * Destructor for the Negotiate Card
 */
NegotiateCard::~NegotiateCard() = default;

/**
 * Playing the block card
 * @param issuer The player playing the card
 */
bool BlockadeCard::play(Player *issuer) const {
  auto ge = GameEngine::instance();
  while (true) {
    cout << "Territories you own: " << endl;
    for (Territory *playerTerritory : issuer->ownedTerritories) {
      cout << "Name: " << playerTerritory->getName() << ", ID : " << playerTerritory->getId() << endl;
    }
    auto territory =
        ge->map->getInputTerritory("Please input the ID of the territory you will Blockade.");
    if (territory->getOwner() != issuer) {
      cout << "Error: Cannot blockade territory you don't own!" << endl << endl;
      continue;
    }
    cout << endl;
    BlockadeCard::execute(issuer, territory);
    return true;
  }
}

/**
 * add blockade card to the orders
 * @param issuer The player playing the card
 * @param territory The territory the card targets
 */
void BlockadeCard::execute(Player *issuer, Territory *territory) {
  auto order = new BlockadeOrder(issuer, territory);
  issuer->orders->push(order);
}

/**
 * Put a card in the deck
 * @param card The card that will be added to the deck
 */
void Deck::put(Card *card) {
  this->cards.push_back(card);
}

/**
 * Playing the airlift card
 * @param issuer The player playing the card
 */
bool AirliftCard::play(Player *issuer) const {
  auto ge = GameEngine::instance();
  while (true) {
    cout << "Territories you own: " << endl;
    for (Territory *territory : issuer->ownedTerritories) {
      cout << "Name: " << territory->getName() << ", ID : " << territory->getId() << endl;
    }
    auto territoryOrigin = ge->map->getInputTerritory(
        "Please input the ID of the territory you will airlift.");
    if (territoryOrigin->getOwner() != issuer) {
      cout << "Error: Cannot airlift from a territory you don't own!" << endl << endl;
      continue;
    }
    auto territoryTarget = ge->map->getInputTerritory(
        "Please input the ID of the destination territory");
    if (territoryTarget->getOwner() != issuer) {
      cout << "Error: Cannot airlift to a territory you don't own!" << endl << endl;
      continue;
    }
    int armiesSize;
    while (true) {
      cout << "Number of soldiers on the chosen territory: " << territoryOrigin->getArmies() << endl;
      armiesSize = Utils::getInputInt("Please input the number of soldiers you wish to move or input -1 to exit");
      if (armiesSize == -1) {
        throw Utils::CancelledInputException();
      }
      if (armiesSize < 0) {
        cout << "Error: Please place a positive army size!" << endl;
        continue;
      }
      if (armiesSize > territoryOrigin->getArmies()) {
        cout << "Error: Number inputted is bigger than army size on this territory" << endl;
        continue;
      }
      break;
    }
    cout << endl;
    AirliftCard::execute(issuer, armiesSize, territoryOrigin, territoryTarget);
    return true;
  }
}

/**
 * add airlift card to the orders
 * @param issuer The player playing the card
 * @param armiesSize The number of troops you want to airlift
 * @param territoryOrigin The territory you will airlift from
 * @param territoryTarget The territory you will airlift to
 */
void AirliftCard::execute(Player *issuer, int armiesSize, Territory *territoryOrigin, Territory *territoryTarget) {
  auto order = new AirliftOrder(issuer, armiesSize, territoryOrigin, territoryTarget);
  issuer->orders->push(order);
}

/**
 * Draw a card from the deck and add it to your hand
 */
Card *Hand::draw() {
  auto ge = GameEngine::instance();
  auto card = ge->deck->draw();
  if (card == nullptr) {
    cout << "There are no more cards to draw in the deck!" << endl;
    return nullptr;
  }
  this->add(card);
  return card;
}

/**
 * Hand copy constructor
 * @param hand The hand argument
 */
Hand::Hand(const Hand &hand) {
  std::vector<Card *> copyCards;
  for (auto card : hand.cards) {
    copyCards.push_back(card);
  }
  this->cards = copyCards;
}

/**
 * Playing the negotiate card
 * @param issuer The player playing the card
 */
bool NegotiateCard::play(Player *issuer) const {
  auto ge = GameEngine::instance();
  std::string playerName;
  Player *target;
  while (true) {
    cout << "List of all players in the game:" << endl;
    for (auto player : ge->players) {
      cout << "- " << player->name << endl;
    }
    playerName = Utils::getInputString(
        "Please input the name of the Player you wish to negotiate with or input \"cancel\" to cancel this card runGameLoop.");
    if (Utils::isEqualLowercase(playerName, "cancel")) {
      return false;
    }
    target = ge->findPlayerByName(playerName);
    if (!target) {
      cout << "Error: this player does not exist!" << endl;
      continue;
    }
    if (target == issuer) {
      cout << "Error: cannot negotiate with yourself!" << endl;
      continue;
    }
    NegotiateCard::execute(issuer, target);
    return true;
  }
}

/**
 * add negotiate card to the orders
 * @param issuer The player asking to negotiate
 * @param target The target player wants to negotiate with
 */
void NegotiateCard::execute(Player *issuer, Player *target) {
  auto order = new NegotiateOrder(issuer, target);
  issuer->orders->push(order);
}

/**
 * Draw a card randomly from the deck
 * @return The card that has been drawn
 */
Card *Deck::draw() {
  if (this->getCardsSize() == 0) {
    return nullptr;
  }

  unsigned int deckSize = this->getCardsSize();
  srand(time(nullptr));
  auto randomLocation = rand() % deckSize;
  auto card = this->cards[randomLocation];
  this->cards.erase(this->cards.begin() + randomLocation);
  return card;
}

/**
 * Get the number of cards in the deck
 * @return the number of cards in the deck
 */
unsigned long long Deck::getCardsSize() {
  return cards.size();
}

/**
 * Deck copy constructor
 * @param deck argument
 */
Deck::Deck(const Deck &deck) {
  std::vector<Card *> copyCards;
  for (auto card : deck.cards) {
    copyCards.push_back(card);
  }
  this->cards = copyCards;
}

/**
 * Operator overload for = on Hand
 * @param hand Right hand side argument
 * @return Deep copy of right hand side hand argument
 */
Hand &Hand::operator=(const Hand &hand) {
  std::vector<Card *> copyCards;
  for (auto card : hand.cards) {
    copyCards.push_back(card);
  }
  this->cards = copyCards;
  return *this;
}

/**
 * Operator overload for = on Deck
 * @param deck Right hand side argument
 * @return Deep copy of right hand side deck argument
 */
Deck &Deck::operator=(const Deck &deck) {
  std::vector<Card *> copyCards;
  for (auto card : deck.cards) {
    copyCards.push_back(card);
  }
  this->cards = copyCards;
  return *this;
}

/**
 * Hand destructor
 */
Hand::~Hand() {
  for (auto card : this->cards) {
    delete (card);
  }
}

Card *Hand::removeByName(const string &name) {
  for (auto card : cards) {
    for (const auto &alias : card->getAliases())
      if (Utils::isEqualLowercase(Utils::trim(name), alias)) {
        remove(card);
        return card;
      }
  }
  return nullptr;
}

/**
 * Play a card from the deck and add it to the deck
 * @param cardName The name of the card
 */
void Hand::play(const string &cardName) {
  auto ge = GameEngine::instance();
  auto card = removeByName(cardName);
  if (card != nullptr) {
    card->play(player);
    ge->deck->put(card);
  } else {
    cout << "Could not find that card. Are you sure you have it in your hand?" << endl;
  }
}

Hand::Hand(Player *player) : player(player) {}

/// Removes a card from the hand and puts in the deck without running the "play" function. Useful if you want to test the Hand functions without user input. Identical to Hand::play.
/// \param name name of card to play
void Hand::debugPlay(const string &name) {
  auto ge = GameEngine::instance();
  auto card = removeByName(name);
  if (card != nullptr) {
//    card->play(player);
    ge->deck->put(card);
  } else {
    cout << "Could not find that card. Are you sure you have it in your hand?" << endl;
  }
}

/**
 * Deck destructor
 */
Deck::~Deck() {
  int counter = 0;
  for (auto card : this->cards) {
    delete (card);
    this->cards[counter] = nullptr;
    counter = counter + 1;
  }
}
