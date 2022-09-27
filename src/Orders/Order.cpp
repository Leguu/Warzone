#include "Order.h"

#include <utility>
#include <iostream>


// ------------------ Order -------------------------
Order::Order(Player *issuer, std::string name, std::string description)
        : name(std::move(name)), description(std::move(description)), issuer(issuer) {}

std::ostream &operator<<(std::ostream &os, const Order &order) {
    os << order.name << ": " << order.description;
    return os;
}

Order::~Order() = default;


// ------------------ DeployOrder ------------------------
DeployOrder::DeployOrder(Player *issuer, int reinforcements, Territory *target)
        : Order(issuer, "Deploy",
                issuer->name + " deploys " + std::to_string(reinforcements) + " armies to " +
                target->toString()),
          reinforcements(reinforcements), target(target) {}

DeployOrder::~DeployOrder() = default;


// ------------------ AdvanceOrder ------------------------
AdvanceOrder::AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Advance",
                issuer->name + " advances " + std::to_string(armies) + " armies from " + source->toString() +
                " to " +
                target->toString()),
          armies(armies), source(source), target(target) {}

AdvanceOrder::~AdvanceOrder() = default;


// ------------------ BombOrder ------------------------
BombOrder::BombOrder(Player *issuer, Territory *target)
	: Order(issuer, "Bomb", issuer->name + " bombs " + target->toString()),
	  target(target) {}

void BombOrder::execute() {
	if (target->getOwner() && (target->getOwner()->name == issuer->name)) {
		throw InvalidOrderException(issuer->name + " attempts to bomb himself! What an idiot.");
	}
	//  this->target->armies /= 2;
	this->target->setArmies(this->target->getArmies()/2);
}

BombOrder::~BombOrder() = default;


// ------------------ BlockadeOrder ------------------------
BlockadeOrder::BlockadeOrder(Player *issuer, Territory *target)
        : Order(issuer, "Blockade", issuer->name + " blockades " + target->toString()),
          target(target) {}

void BlockadeOrder::execute() {
	//  TODO:
	//  -----
	// 	     1. Validate/throw under some condition.
	this->target->setArmies(this->target->getArmies() * 3);
	//       2. "make it a neutral territory": The neutral armies cannot move, and simply sit still for the entire
	//          duration of the game until conquered by a player.
}

BlockadeOrder::~BlockadeOrder() = default;


// ------------------ AirliftOrder ------------------------
AirliftOrder::AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Airlift",
                issuer->name + " airlifts " + std::to_string(armies) + " armies from " + source->toString() +
                " to " +
                target->toString()),
          armies(armies), source(source), target(target) {}

NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
        : Order(issuer, "Negotiate", issuer->name + " negotiates with " + target->name),
          target(target) {}


// ------------------ OrderList ------------------------
void OrderList::push(Order* order) {
	this->orders.push_back(order);
}

Order* OrderList::pop() {
	if (this->orders.empty()) return nullptr;
	auto order = this->orders.front();
	this->orders.pop_front();
	return order;
}


// ------------------ Exception ------------------------
InvalidOrderException::InvalidOrderException(const std::string &arg) : runtime_error(arg) {}
