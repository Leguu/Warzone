//
// Created by Legu on 2022-09-13.
//

#include <iostream>
#include <utility>
#include "Card.h"
#include "../GameEngine.h"
#include "../Utils/Utils.h"

std::ostream &operator<<(std::ostream &os, const Hand &hand) {
    os << "You have " << hand.cards.size() << " cards in your hand." << std::endl;
    for (auto card: hand.cards) {
        os << card << std::endl;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Deck &deck) {
    os << "You have " << deck.cards.size() << " cards in your deck." << std::endl;
    for (auto card: deck.cards) {
        os << card << std::endl;
    }
    return os;
}

const Card *Deck::draw() {
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
    this->hand.cards.push_back(card);
}

std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << card.name << " Card: " << card.description;
    return os;
}

Card::Card(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}

void BombCard::play() const {
    auto ge = GameEngine::instance();
    auto territoryId = Utils::getInputInt("Please input the ID of the territory you will bomb");
    auto territory = ge->map->findById(territoryId);
    if (!territory) {
        std::cout << "Error: this territory does not exist!" << std::endl;
        return;
    }
    const auto order = new BombOrder(territory);
    ge->orders.push(order);
}
