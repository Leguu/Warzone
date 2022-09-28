#include "Order.h"

#include <utility>
#include <iostream>
#include <iomanip>

using namespace std;

// ------------------ Order -------------------------
Order::Order(Player *issuer, string name, string description)
	: name(move(name)), description(move(description)), issuer(issuer) {}

ostream &operator<<(ostream &os, const Order &order) {
  os << order.name << ": " << order.description;
  return os;
}

Order::~Order() = default;

// ------------------ DeployOrder ------------------------
DeployOrder::DeployOrder(Player *issuer, int reinforcements, Territory *target)
	: Order(issuer, "Deploy",
			issuer->name + " deploys " + to_string(reinforcements) + " armies to " +
				target->toString()),
	  reinforcements(reinforcements), target(target) {}

void DeployOrder::validate() {// Implement check if is within territory
  if (target->getOwner() && (target->getOwner()->ownedTerritories != issuer->ownedTerritories)) {
	throw InvalidOrderException(issuer->name + "tried to deploy in enemy territory!");
  }
}

void DeployOrder::execute() {
  validate();
  this->target->setArmies(this->target->getArmies() + reinforcements);
}

DeployOrder::~DeployOrder() = default;

// ------------------ AdvanceOrder ------------------------
AdvanceOrder::AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target)
	: Order(issuer, "Advance",
			issuer->name + " advances " + to_string(armies) + " armies from " + source->toString() +
				" to " +
				target->toString()),
	  armies(armies), source(source), target(target) {}

AdvanceOrder::~AdvanceOrder() = default;

// ------------------ BombOrder ------------------------
BombOrder::BombOrder(Player *issuer, Territory *target)
	: Order(issuer, "Bomb", issuer->name + " bombs " + target->toString()),
	  target(target) {}

void BombOrder::validate() {
  // Validation:
  // -----------
  //           1. If player attempts to bomb himself.
  if (target->getOwner() && (target->getOwner()->name == issuer->name)) {
	throw InvalidOrderException(issuer->name + " attempts to bomb himself! What an idiot.");
  }
}

void BombOrder::execute() {
  validate();
  //  this->target->armies /= 2;
  this->target->setArmies(this->target->getArmies() / 2);
}

BombOrder::~BombOrder() = default;

// ------------------ BlockadeOrder ------------------------
BlockadeOrder::BlockadeOrder(Player *issuer, Territory *target)
	: Order(issuer, "Blockade", issuer->name + " blockades " + target->toString()),
	  target(target) {}

void BlockadeOrder::validate() {
  // Validation:
  // -----------
  //           1. If player does not own territory? (To confirm if this is a possibility)
  if (find(issuer->ownedTerritories.begin(), issuer->ownedTerritories.end(), target)
	  != issuer->ownedTerritories.end()) {
	throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
  }
}
void BlockadeOrder::execute() {
  validate();

  this->target->setArmies(this->target->getArmies() * 3);
  //  TODO:
  //  -----
  //         1. "make it a neutral territory": The neutral armies cannot move, and simply sit still for the entire
  //            duration of the game until conquered by a player.
}

BlockadeOrder::~BlockadeOrder() = default;

// ------------------ AirliftOrder ------------------------
AirliftOrder::AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target)
	: Order(issuer, "Airlift",
			issuer->name + " airlifts " + to_string(armies) + " armies from " + source->toString() +
				" to " +
				target->toString()),
	  armies(armies), source(source), target(target) {}

void AirliftOrder::validate() {
  // Validation:
  // -----------
  //           1. If source territory does not have int "armies" number of army members.
  //           2. If player does not own source territory? (To confirm if this is a possibility)
  if (source->getArmies() - armies < 0) {
	throw InvalidOrderException(
		issuer->name + "'s source territory (" + source->getName() + ") does not have " + to_string(armies)
			+ " number of army members.");
  } else if (find(issuer->ownedTerritories.begin(), issuer->ownedTerritories.end(), target)
	  != issuer->ownedTerritories.end()) {
	throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
  }
}

void AirliftOrder::execute() {
  validate();

  int sourceArmies = this->source->getArmies();
  this->source->setArmies(sourceArmies - armies);
  this->target->setArmies(sourceArmies + armies);
}

AirliftOrder::~AirliftOrder() = default;

// ------------------ NegotiateOrder ------------------------
NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
	: Order(issuer, "Negotiate", issuer->name + " negotiates with " + target->name),
	  target(target) {}

// ------------------ OrderList ------------------------
void OrderList::push(Order *order) {
  this->orders.push_back(order);
}

Order *OrderList::pop() {
  if (this->orders.empty())
	return nullptr;
  auto order = this->orders.front();
  this->orders.pop_front();
  return order;
}

void OrderList::remove(int index) {
  auto iter = this->orders.begin();
  advance(iter, index);
  this->orders.erase(iter);
}

void OrderList::move(int a, int b) {
  auto iterA = this->orders.begin();
  advance(iterA, a);

  auto iterB = this->orders.begin();
  advance(iterB, b);

  auto tmp = *iterA;
  *iterA = *iterB;
  *iterB = tmp;
}

ostream &operator<<(ostream &os, const OrderList &orderList) {
  auto idx = 7;
  auto divider = "--------------------------------------------------------------------------------";

  cout << divider << "\n" << left << setw(3) << "Id | " << left << setw(15) << "Name"
	   << "| Description"
	   << "\n" << divider << endl;

  for (auto iter = orderList.orders.begin(); iter != orderList.orders.end(); ++iter) {
	cout << left << setw(3) << to_string(idx) << "| " << left << setw(15)
		 << (*iter)->getName()
		 << "| " + (*iter)->description
		 << endl;
	++idx;
  }

  cout << divider << endl;

  return os;
}

// ------------------ Exception ------------------------
InvalidOrderException::InvalidOrderException(const string &arg) : runtime_error(arg) {}
