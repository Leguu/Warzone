#include <iostream>
#include <utility>
#include <memory>
#include "Card.h"
//Having this as per teacher's recommendation never to have a class base its references on a reference references
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
        os << card << std::endl;
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
 * Constructor for a Card object
 * @param name The name of the card
 * @param description The description of the card
 */
Card::Card(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}

/**
 * Destructor for the card
 */
Card::~Card() = default;

/**
 * Playing the bomb card
 * @param issuer The player playing the card
 */
void BombCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto territoryId = Utils::getInputInt("Please input the ID of the territory you will bomb");
    auto territory = ge->map->findById(territoryId);
    if (!territory) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
    if (territory->owner == issuer) {
        std::cout << "Error: Cannot bomb your own territory!" << std::endl;
        return;
    }
    auto order = std::make_unique<BombOrder>(issuer, territory);
    issuer->orders->push(std::move(order));
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
void BlockadeCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto territoryId = Utils::getInputInt("Please input the ID of the territory you will bomb");
    auto territory = ge->map->findById(territoryId);
    if (!territory) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
    if (territory->owner != issuer) {
        std::cout << "Error: Cannot blockade your own territory!" << std::endl;
        return;
    }
    auto order = std::make_unique<BlockadeOrder>(issuer, territory);
    issuer->orders->push(std::move(order));
}

/**
 * Put a card in the deck
 * @param card The card that will be added to the deck
 */
void Deck::put(Card* card){
    unsigned int deckSize = this->cardCollection.size();
    unsigned int randomLocation = rand() % deckSize;
    this->cardCollection.insert(this->cardCollection.begin() + randomLocation, card);
}

/**
 * Deck constructor
 * @param cardCollection The cards that are initially in the deck
 */
Deck::Deck(std::vector<Card *> cardCollection) {
    this->cardCollection = std::move(cardCollection);
}

/**
 * Playing the airlift card
 * @param issuer The player playing the card
 */
void AirliftCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto territoryPlayerId = Utils::getInputInt("Please input the ID of the territory you will airlift from");
    auto territoryPlayer = ge->map->findById(territoryPlayerId);
    if (!territoryPlayer) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
    auto armiesSize = Utils::getInputInt("Please input the number of soldiers you wish to move");
    if (armiesSize < 0) {
        std::cout << "Error: Please place a positive army size!" << std::endl;
        return;
    }
    if (armiesSize > territoryPlayer->armies) {
        std::cout << "Error: Please do write a number smaller than the total army size on this territory!" << std::endl;
        return;
    }
    auto territoryTargetId = Utils::getInputInt("Please input the ID of the territory you will airlift to");
    auto territoryTarget = ge->map->findById(territoryTargetId);
    if (!territoryTarget) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
//    if (!territoryTarget->owner || territoryTarget->owner != territoryPlayer->owner) {
//        std::cout << "Error: Please make sure the target territory belongs to you!" << std::endl;
//        return;
//    }
    auto order = std::make_unique<AirliftOrder>(issuer, armiesSize, territoryPlayer, territoryTarget);
    issuer->orders->push(std::move(order));
}

/**
 * Playing the negotiate card
 * @param issuer The player playing the card
 */
void NegotiateCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto playerName = Utils::getInputString("Please input the name of the Player you wish to negotiate with");
    Player *player = ge->findPlayerByName(playerName);
    if (!player) {
        std::cout << "Error: this player does not exist!" << std::endl;
        return;
    }
    auto order = std::make_unique<NegotiateOrder>(issuer, player);
    issuer->orders->push(std::move(order));
}

/**
 * Draw a card from the deck
 * @return The card that has been drawn
 */
Card* Deck::draw() {
    auto* card = this->cardCollection.back();
    this->cardCollection.pop_back();
    return card;
}
