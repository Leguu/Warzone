#include "Order.h"
#include "../GameEngine/GameEngine.h"

#include <utility>
#include <iostream>
#include <iomanip>

// ------------------ Order -------------------------
Order::Order(Player *issuer, std::string name)
    : name(std::move(name)), issuer(issuer) {}

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

void DeployOrder::validate() {
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

DeployOrder::DeployOrder(const DeployOrder &other) : Order(other.issuer, other.name),
                                                     reinforcements(other.reinforcements), target(other.target) {
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
AdvanceOrder::AdvanceOrder(const AdvanceOrder &other)
    : Order(other.issuer, other.name), armies(other.armies), source(other.source), target(other.target) {}

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

BombOrder::BombOrder(const BombOrder &other) : Order(other.issuer, other.name), target(other.target) {}

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

BlockadeOrder::BlockadeOrder(const BlockadeOrder &other) : Order(other.issuer, other.name), target(other.target) {}

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
AirliftOrder::AirliftOrder(const AirliftOrder &other) : Order(other.issuer, other.name), target(other.target),
                                                        source(other.source), armies(other.armies) {}

AirliftOrder::~AirliftOrder() = default;

// ------------------ NegotiateOrder ------------------------
NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
    : Order(issuer, "Negotiate"),
      target(target) {}

std::string NegotiateOrder::description() {
  return issuer->name + " negotiates with " + target->name;
}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &o) : Order(o.issuer, o.name), target(o.target) {}

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
  auto order = orders[index];
  this->orders.erase(this->orders.begin() + index);
  delete order;
}

void OrderList::move(int a, int b) {
  if ((0 <= a && a < this->orders.size()) && (0 <= b && b < this->orders.size())) {
    std::swap(this->orders[a], this->orders[b]);
  } else {
    throw runtime_error("Index out of bounds!");
  }
}

void OrderList::executeOrders() {
  while (!this->orders.empty()) {
    auto order = pop();
    order->execute();
    delete order;
  }
}

int OrderList::getOrdersSize() {
  return orders.size();
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

OrderList::OrderList(const OrderList &o) {
  orders = o.orders;
}

OrderList::OrderList() = default;

// ------------------ Exception ------------------------
InvalidOrderException::InvalidOrderException(const std::string &arg) : runtime_error(arg) {}

void testOrdersLists() {
  OrderList list = OrderList();

  auto ge = new GameEngine("../assets/Moon.map");
  const auto territoryS = ge->map->findTerritory("Byrgius");
  const auto territoryT = ge->map->findTerritory("Bay of dew");
  auto player1 = Player("Bob");
  auto player2 = Player("Alice");

  auto deploy = new DeployOrder(&player1, 0, territoryT);
  auto advance = new AdvanceOrder(&player1, 5, territoryS, territoryT);
  auto bomb = new BombOrder(&player1, territoryT);
  auto blockade = new BlockadeOrder(&player1, territoryT);
  auto airlift = new AirliftOrder(&player1, 0, territoryS, territoryT);
  auto negotiate = new NegotiateOrder(&player1, &player2);

  list.push(deploy);
  list.push(advance);
  list.push(bomb);
  list.push(blockade);
  list.push(airlift);
  list.push(negotiate);
  Utils::assertCondition(list.getOrdersSize() == 6, "Push operation failed: list size is not 6.");

  list.remove(2);
  Utils::assertCondition(list.getOrdersSize() == 5, "Remove operation failed: list size is not 5.");
  Utils::assertCondition(list.get(2) == blockade, "Remove operation failed: index 2 is not blockade order.");

  list.move(1, 2);
  Utils::assertCondition(list.get(1) == blockade, "Move operation failed: index 1 is not blockade order.");
  Utils::assertCondition(list.get(2) == advance, "Move operation failed: index 2 is not advance order.");

  list.executeOrders();
  Utils::assertCondition(list.getOrdersSize() == 0, "Execute orders operation failed: list is not empty.");

  cout << "All tests passed successfully!" << endl;
  delete ge;
}
