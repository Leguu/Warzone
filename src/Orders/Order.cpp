#include "Order.h"

#include <utility>
#include <iostream>
#include <iomanip>

// ------------------ Order -------------------------
Order::Order(Player *issuer, std::string name)
	: name(std::move(name)),issuer(issuer) {}

std::ostream &operator<<(std::ostream &os, Order &order) {
  os << order.name << ": " << order.description();
  return os;
}


Order::~Order() = default;

// ------------------ DeployOrder ------------------------
DeployOrder::DeployOrder(Player *issuer, int reinforcements, Territory *target)
	: Order(issuer, "Deploy"),
	  reinforcements(reinforcements), target(target) {}
std::string DeployOrder::description() {
  return issuer->name + " deploys " + std::to_string(reinforcements) + " armies to " +
      target->toString();
}
void DeployOrder::validate() {// Implement check if is within territory
  if (target->getOwner() && (target->getOwner() != issuer)) {
	throw InvalidOrderException(issuer->name + " tried to deploy in someone else's territory.");
  } else if (reinforcements > issuer->reinforcements) {
	throw InvalidOrderException(issuer->name + " does not have the specified number of reinforcements. Player has "
									+ std::to_string(issuer->reinforcements) + " reinforcements, but requested "
									+ std::to_string(reinforcements) + " reinforcements.");
  }
}

void DeployOrder::execute() {
  validate();
  this->issuer->reinforcements -= reinforcements;
  this->target->setArmies(this->target->getArmies() + reinforcements);
}

DeployOrder::~DeployOrder() = default;

// ------------------ AdvanceOrder ------------------------
AdvanceOrder::AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target)
	: Order(issuer, "Advance"),
	  armies(armies), source(source), target(target) {}
std::string AdvanceOrder::description() {
  return issuer->name + " advances " + std::to_string(armies) + " armies from " + source->toString() +
      " to " +
      target->toString();
}

AdvanceOrder::~AdvanceOrder() = default;

// ------------------ BombOrder ------------------------
BombOrder::BombOrder(Player *issuer, Territory *target)
	: Order(issuer, "Bomb"),
	  target(target) {}
std::string BombOrder::description() {
  return issuer->name + " bombs " + target->toString();
}

void BombOrder::validate() {
  if (target->getOwner() && (target->getOwner()->name == issuer->name)) {
	throw InvalidOrderException(issuer->name + " attempts to bomb himself! What an idiot.");
  }
}

void BombOrder::execute() {
  validate();
  this->target->setArmies(this->target->getArmies() / 2);
}

BombOrder::~BombOrder() = default;

// ------------------ BlockadeOrder ------------------------
BlockadeOrder::BlockadeOrder(Player *issuer, Territory *target)
	: Order(issuer, "Blockade"),
	  target(target) {}
std::string BlockadeOrder::description() {
  return issuer->name + " blockades " + target->toString();
}
void BlockadeOrder::validate() {
  if (target->getOwner() && (target->getOwner() != issuer)) {
	throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
  }
}
void BlockadeOrder::execute() {
  validate();
  this->target->setArmies(this->target->getArmies() * 3);
}

BlockadeOrder::~BlockadeOrder() = default;

// ------------------ AirliftOrder ------------------------
AirliftOrder::AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target)
	: Order(issuer, "Airlift"),
	  armies(armies), source(source), target(target) {}
std::string AirliftOrder::description() {
  return issuer->name + " airlifts " + std::to_string(armies) + " armies from " + source->toString() +
      " to " +
      target->toString();
}
void AirliftOrder::validate() {
  if (source->getArmies() - armies < 0) {
	throw InvalidOrderException(
		issuer->name + "'s source territory (" + source->getName() + ") does not have " + std::to_string(armies)
			+ " number of army members.");
  } else if (target->getOwner() && (target->getOwner() != issuer)) {
	throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
  }
}

void AirliftOrder::execute() {
  validate();
  int sourceArmies = this->source->getArmies();
  int targetArmies = this->target->getArmies();
  this->source->setArmies(sourceArmies - armies);
  this->target->setArmies(targetArmies + armies);
}

AirliftOrder::~AirliftOrder() = default;

// ------------------ NegotiateOrder ------------------------
NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
	: Order(issuer, "Negotiate"),
	  target(target) {}
std::string NegotiateOrder::description() {
  return issuer->name + " negotiates with " + target->name;
}
NegotiateOrder::~NegotiateOrder() = default;

// ------------------ OrderList ------------------------
void OrderList::push(Order *order) {
  this->orders.push_back(order);
}

Order *OrderList::pop() {
  if (this->orders.empty())
	return nullptr;
  auto order = this->orders.front();
  this->orders.erase(this->orders.begin());
  return order;
}

Order *OrderList::get(int index) {
  return this->orders[index];
}

void OrderList::remove(int index) {
  this->orders.erase(this->orders.begin() + index);
}

void OrderList::move(int a, int b) {
  if ((0 <= a && a < this->orders.size()) && (0 <= b && b < this->orders.size())) {
	std::swap(this->orders[a], this->orders[b]);
  }
}

void OrderList::executeOrders() {
  while (!this->orders.empty()) {
	auto order = pop();
	order->execute();
  }
}

int OrderList::getNumberOfOrders() {
  return this->orders.size();
}

std::ostream &operator<<(std::ostream &os, const OrderList &orderList) {
  auto i = 1;
  auto divider = "--------------------------------------------------------------------------------";

  std::cout << divider << "\n" << std::left << std::setw(3) << "Id | " << std::left << std::setw(10) << "Name"
			<< "| Description"
			<< "\n" << divider << std::endl;

  for (auto order : orderList.orders) {
	std::cout << std::left << std::setw(3) << std::to_string(i) << "| " << std::left << std::setw(10)
			  << order->name
			  << "| " + order->description()
			  << std::endl;
	++i;
  }

  std::cout << divider << std::endl;
  return os;
}

// ------------------ Exception ------------------------
InvalidOrderException::InvalidOrderException(const std::string &arg) : runtime_error(arg) {}
