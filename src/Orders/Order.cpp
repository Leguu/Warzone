#include "Order.h"

#include <utility>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Order &order) {
    os << order.name << ": " << order.description;
    return os;
}

Order::~Order() = default;

void BombOrder::execute() {
    if (target->getOwner() && (target->getOwner()->name == issuer->name)) {
        throw InvalidOrderException(issuer->name + " attempts to bomb himself! What an idiot.");
    }
  //  this->target->armies /= 2;
    this->target->setArmies(this->target->getArmies()/2);
}

BombOrder::BombOrder(Player *issuer, Territory *target)
        : Order(issuer, "Bomb", issuer->name + " bombs " + target->toString()),
          target(target) {}

BombOrder::~BombOrder() {
}

InvalidOrderException::InvalidOrderException(const std::string &arg) : runtime_error(arg) {}

void OrderList::push(Order* order) {
    this->orders.push_back(order);
}

Order* OrderList::pop() {
    if (this->orders.empty()) return nullptr;
    auto order = this->orders.front();
    this->orders.pop_front();
    return order;
}

/**
 * Return the number of orders the player has used
 * @return The number of orders
 */
int OrderList::getOrdersSize() {
    return this->orders.size();
}

DeployOrder::DeployOrder(Player *issuer, int reinforcements, Territory *target)
        : Order(issuer, "Deploy",
                issuer->name + " deploys " + std::to_string(reinforcements) + " armies to " +
                target->toString()),
          reinforcements(reinforcements), target(target) {}

DeployOrder::~DeployOrder() {

}

AdvanceOrder::AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Advance",
                issuer->name + " advances " + std::to_string(armies) + " armies from " + source->toString() +
                " to " +
                target->toString()),
          armies(armies), source(source), target(target) {}

AdvanceOrder::~AdvanceOrder() {

}

BlockadeOrder::BlockadeOrder(Player *issuer, Territory *target)
        : Order(issuer, "Blockade", issuer->name + " blockades " + target->toString()),
          target(target) {}

AirliftOrder::AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Airlift",
                issuer->name + " airlifts " + std::to_string(armies) + " armies from " + source->toString() +
                " to " +
                target->toString()),
          armies(armies), source(source), target(target) {}

NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
        : Order(issuer, "Negotiate", issuer->name + " negotiates with " + target->name),
          target(target) {}
