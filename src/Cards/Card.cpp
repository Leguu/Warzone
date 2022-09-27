#include <iostream>
#include <utility>
#include <memory>
#include "Card.h"
#include "../Player/Player.h"
#include "../GameEngine/GameEngine.h"
#include "../Utils/Utils.h"
#include <cstdlib>

/**
 * Function to print to console the content's of one's hand
 * @param os The OSStream we will be appending to
 * @param hand  The player hand we will be examining
 * @return A string containing the content's of the player's hand
 */
std::ostream &operator<<(std::ostream &os, const Hand &hand) {
    os << "You have " << hand.cards.size() << " cards in your hand." << std::endl;
    for (auto &card: hand.cards) {
        os << *card << std::endl;
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
    os << "You have " << deck.cards.size() << " cards in your deck." << std::endl;
    for (auto &card: deck.cards) {
        os << *card << std::endl;
    }
    return os;
}

/**
 * List the contents of the hand
 */
void Hand::listHand() {
    std::cout << this << std::endl;
}

/**
 * Remove a card from the hand
 * @param card The card that is removed
 */
void Hand::remove(int index) {
    this->cards.erase(this->cards.begin() + index);
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
 * Destructor for the card
 */
Card::~Card() = default;

/**
 * Playing the bomb card
 * @param issuer The player playing the card
 */
bool BombCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    int territoryId = -1;
    Territory *territory = nullptr;
    auto adjacentEnemyTerritories = issuer->getAdjacentEnemyTerritories();
    bool validInput = false;
    while (!validInput) {
        std::cout << "Adjacent enemy territories you can bomb : " << std::endl;
        for (auto *adjacentEnemyTerritory: adjacentEnemyTerritories) {
            std::cout << "Name: " << adjacentEnemyTerritory->getName() << ", ID : " << adjacentEnemyTerritory->getId()
                      << std::endl;
        }
        territoryId = Utils::getInputInt("Please input the ID of the territory you will bomb or input -1 to exit\n");
        if (territoryId == -1) {
            return false;
        }
        territory = ge->map->findById(territoryId);
        if (!territory) {
            std::cout << "Error: this territory does not exist!\n" << std::endl;
            continue;
        }
        if (territory->getOwner() == issuer) {
            std::cout << "Error: Cannot bomb your own territory!\n" << std::endl;
            continue;
        }
        bool territoryIsAdjacent = false;
        for (auto *adjacentEnemyTerritory: adjacentEnemyTerritories) {
            if (territoryId == adjacentEnemyTerritory->getId())
                territoryIsAdjacent = true;
        }
        if (!territoryIsAdjacent) {
            std::cout << "Error: Can only bomb adjacent territories!\n" << std::endl;
            continue;
        }
        auto order = new BombOrder(issuer, territory);
        issuer->orders->push(order);
        std::cout << std::endl;
        validInput = true;
    }
    return true;
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
    int territoryId;
    Territory* territory = nullptr;
    bool validInput = false;
    while (!validInput) {
        std::cout << "Territories you own: " << std::endl;
        for(Territory* playerTerritory: issuer->ownedTerritories){
            std::cout << "Name: " << playerTerritory->getName() << ", ID : " << playerTerritory->getId() << std::endl;
        }
        territoryId = Utils::getInputInt("Please input the ID of the territory you will Blockade or input -1 to exit \n");
        if (territoryId == -1) {
            return false;
        }
        territory = ge->map->findById(territoryId);
        if (!territory) {
            std::cout << "Error: This territory does not exist!\n" << std::endl;
            continue;
        }
        if (territory->getOwner() != issuer) {
            std::cout << "Error: Cannot blockade territory you don't own!\n" << std::endl;
            continue;
        }
        auto order = new BlockadeOrder(issuer, territory);
        issuer->orders->push(order);
        std::cout << std::endl;
        validInput = true;
    }
    return true;
}

/**
 * Put a card in the deck
 * @param card The card that will be added to the deck
 */
void Deck::put(Card *card) {
    unsigned int randomLocation = this->getRandomLocation();
    this->cards.insert(this->cards.begin() + randomLocation, card);
}


/**
 * Playing the airlift card
 * @param issuer The player playing the card
 */
bool AirliftCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto territoryPlayerId = Utils::getInputInt("Please input the ID of the territory you will airlift from");
    auto territoryPlayer = ge->map->findById(territoryPlayerId);
    if (!territoryPlayer) {
        std::cout << "Error: this territory does not exist!" << std::endl;
    }
    auto armiesSize = Utils::getInputInt("Please input the number of soldiers you wish to move");
    if (armiesSize < 0) {
        std::cout << "Error: Please place a positive army size!" << std::endl;
    }
    if (armiesSize > territoryPlayer->getArmies()) {
        std::cout << "Error: Please do write a number smaller than the total army size on this territory!" << std::endl;
    }
    auto territoryTargetId = Utils::getInputInt("Please input the ID of the territory you will airlift to");
    auto territoryTarget = ge->map->findById(territoryTargetId);
    if (!territoryTarget) {
        std::cout << "Error: this territory does not exist!" << std::endl;
    }
    auto order = new AirliftOrder(issuer, armiesSize, territoryPlayer, territoryTarget);
    issuer->orders->push(order);
    return true;
}

/**
 * Draw a card from the deck and add it to your hand
 */
Card *Hand::draw() {
    auto ge = GameEngine::instance();
    auto card = ge->deck->draw();
    this->add(card);
    return card;
}

/**
 * Hand copy constructor
 * @param hand The hand argument
 */
Hand::Hand(const Hand &hand) {
    std::vector<Card *> cards;
    for (auto *card: hand.cards) {
        cards.push_back(card);
    }
    this->cards = cards;
}

/**
 * Playing the negotiate card
 * @param issuer The player playing the card
 */
bool NegotiateCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto playerName = Utils::getInputString("Please input the name of the Player you wish to negotiate with");
    Player *player = ge->findPlayerByName(playerName);
    if (!player) {
        std::cout << "Error: this player does not exist!" << std::endl;
    }
    auto order = new NegotiateOrder(issuer, player);
    issuer->orders->push(order);
    return true;
}

/**
 * Draw a card randomly from the deck
 * @return The card that has been drawn
 */
Card *Deck::draw() {
    unsigned int randomLocation = this->getRandomLocation();
    auto *card = this->cards[randomLocation];
    this->cards.erase(this->cards.begin() + randomLocation);
    return card;
}

int Deck::getCardsSize() {
    return this->cards.size();
}

int Deck::getRandomLocation() {
    unsigned int deckSize = this->getCardsSize();
    if (deckSize > 0) {
        return (rand() % deckSize);
    }
}

/**
 * Deck copy constructor
 * @param deck argument
 */
Deck::Deck(const Deck &deck) {
    std::vector<Card *> cards;
    for (auto *card: deck.cards) {
        cards.push_back(card);
    }
    this->cards = cards;
}

/**
 * Operator overload for = on Hand
 * @param hand Right hand side argument
 * @return Deep copy of right hand side hand argument
 */
Hand &Hand::operator=(const Hand &hand) {
    std::vector<Card *> cards;
    for (auto *card: hand.cards) {
        cards.push_back(card);
    }
    this->cards = cards;
    return *this;
}

/**
 * Operator overload for = on Deck
 * @param deck Right hand side argument
 * @return Deep copy of right hand side deck argument
 */
Deck &Deck::operator=(const Deck &deck) {
    std::vector<Card *> cards;
    for (auto *card: deck.cards) {
        cards.push_back(card);
    }
    this->cards = cards;
    return *this;
}

/**
 * Hand destructor
 */
Hand::~Hand() {
    int counter = 0;
    for (auto *card: this->cards) {
        delete (card);
        this->cards[counter] = nullptr;
        counter = counter + 1;
    }
}

/**
 * Deck destructor
 */
Deck::~Deck() {
    int counter = 0;
    for (auto *card: this->cards) {
        delete (card);
        this->cards[counter] = nullptr;
        counter = counter + 1;
    }
}
