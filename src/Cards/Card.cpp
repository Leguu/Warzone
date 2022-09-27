#include <iostream>
#include <utility>
#include <memory>
#include "Card.h"
#include "../GameEngine/GameEngine.h"
#include "../Utils/Utils.h"

std::ostream &operator<<(std::ostream &os, const Hand &hand) {
    os << "You have " << hand.cards.size() << " cards in your hand." << std::endl;
    for (auto &card: hand.cards) {
        os << *card << std::endl;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Deck &deck) {
    os << "You have " << deck.cards.size() << " cards in your deck." << std::endl;
    for (auto &card: deck.cards) {
        os << *card << std::endl;
    }
    return os;
}

std::unique_ptr<Card> Deck::draw() {
    return nullptr;
}

void CardManager::listHand() {
    std::cout << this->hand << std::endl;
}

void CardManager::listDeck() {
    std::cout << this->deck << std::endl;
}

void CardManager::draw() {
    auto card = this->deck.draw();
    this->hand.cards.push_back(std::move(card));
}

std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << card.name << " Card: " << card.description;
    return os;
}

Card::Card(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}

Card::~Card() = default;

void BombCard::play(Player *issuer) const {
    auto ge = GameEngine::instance();
    auto territoryId = Utils::getInputInt("Please input the ID of the territory you will bomb");
    auto territory = ge->map->findById(territoryId);
    if (!territory) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
    const auto order = new BombOrder(issuer, territory);
    issuer->orders->push(order);
}

BombCard::~BombCard() = default;

BlockadeCard::~BlockadeCard() = default;

AirliftCard::~AirliftCard() = default;

NegotiateCard::~NegotiateCard() = default;

void BlockadeCard::play(Player *issuer) const {

}

void AirliftCard::play(Player *issuer) const {

}

void NegotiateCard::play(Player *issuer) const {

}
