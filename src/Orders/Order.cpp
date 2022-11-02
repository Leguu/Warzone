#include "Order.h"
#include "../GameEngine/GameEngine.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "../Logging/LogObserver.h"

// ------------------ Order -------------------------
/**
 * Order constructor
 * @param issuer The player who owns the order
 * @param name the name of the order
 */
Order::Order(Player *issuer, std::string name)
        : name(std::move(name)), issuer(issuer) {
    this->Attach(LogObserver::instance());
}

/**
 * Function to print to console the content's of the order
 * @param os The OSStream we will be appending to
 * @param hand  The order we will be examining
 * @return A string containing the content's of the order
 */
std::ostream &operator<<(std::ostream &os, Order &order) {
    os << order.name << ": " << order.description();
    return os;
}

/**
 * Log the information of this order
 * @return a String containing the information that will be logged
 */
std::string Order::stringToLog() {
    std::ofstream file;
    file.open("../logs/gamelog.txt", std::ios_base::app);
    file << "Order Executed: \"" << this->name << "\" by Player \"" << this->issuer->name << "\"" <<std::endl << std::endl;
    return "Order Executed: \"" + this->name + "\" by Player \"" + this->issuer->name + "\"";
}

/**
 * Log the information of this order
 * @return a String containing the information that will be logged
 */
std::string OrderList::stringToLog() {
    std::ofstream file;
    auto lastOrder = this->orders[this->getOrdersSize() - 1];
    file.open("../logs/gamelog.txt", std::ios_base::app);
    file << "Order Issued: \"" << lastOrder->name << "\" by Player \"" << lastOrder->issuer->name << "\"" << std::endl << std::endl;
    return "Order Issued: \"" + lastOrder->name + "\" by Player \"" + lastOrder->issuer->name + "\"";
}

/**

 * Order destructor
 */
Order::~Order() {
    this->Detach(LogObserver::instance());
}

// ------------------ DeployOrder ------------------------
/**
 * Deploy order constructor
 * @param issuer The player issuing the order
 * @param reinforcements The reinforcements to deploy
 * @param target The target territory
 */
DeployOrder::DeployOrder(Player *issuer, int reinforcements, Territory *target)
        : Order(issuer, "Deploy"),
          reinforcements(reinforcements), target(target) {}

/**
 * Description of the deploy order
 * @return String containing the description
 */
std::string DeployOrder::description() {
    return issuer->name + " deploys " + std::to_string(reinforcements) + " armies to " +
           target->toString();
}

/**
 * Validate a deploy order
 */
void DeployOrder::validate() {
    if (target->getOwner() && (target->getOwner() != issuer)) {
        throw InvalidOrderException(issuer->name + " tried to deploy in someone else's territory.");
    } else if (reinforcements > issuer->reinforcements) {
        throw InvalidOrderException(issuer->name + " does not have the specified number of reinforcements. Player has "
                                    + std::to_string(issuer->reinforcements) + " reinforcements, but requested "
                                    + std::to_string(reinforcements) + " reinforcements.");
    }
}

/**
 * Execute a deploy order
 */
void DeployOrder::execute() {
    validate();
    this->issuer->reinforcements -= reinforcements;
    this->target->setArmies(this->target->getArmies() + reinforcements);
    this->Notify(this);
}

/**
 * Deploy order copy constructor
 * @param other the to-be-copied deploy order
 */
DeployOrder::DeployOrder(const DeployOrder &other) : Order(other.issuer, other.name),
                                                     reinforcements(other.reinforcements), target(other.target) {
}

/**
 * Deploy order destructor
 */
DeployOrder::~DeployOrder() = default;

// ------------------ AdvanceOrder ------------------------
/**
 * Advance order constructor
 * @param issuer  The player issuing the order
 * @param armies The armies to advance
 * @param source the source territory
 * @param target the target territory
 */
AdvanceOrder::AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Advance"),
          armies(armies), source(source), target(target) {}

/**
 * Description of the advance order
 * @return String with the description of the advance order
 */
std::string AdvanceOrder::description() {
    return issuer->name + " advances " + std::to_string(armies) + " armies from " + source->toString() +
           " to " +
           target->toString();
}

/**
 * Advance order copy constructor
 * @param other The to-be-copied object
 */
AdvanceOrder::AdvanceOrder(const AdvanceOrder &other)
        : Order(other.issuer, other.name), armies(other.armies), source(other.source), target(other.target) {}

/**
 * Destructor for advance order
 */
AdvanceOrder::~AdvanceOrder() = default;

// ------------------ BombOrder ------------------------
/**
 * Bomb order constructor
 * @param issuer The player issuing the bomb order
 * @param target The bomb order target territory
 */
BombOrder::BombOrder(Player *issuer, Territory *target)
        : Order(issuer, "Bomb"),
          target(target) {}

/**
 * Description of the bomb order
 * @return string with the description
 */
std::string BombOrder::description() {
    return issuer->name + " bombs " + target->toString();
}

/**
 * Validate if the bomb order can be executed
 */
void BombOrder::validate() {
    if (target->getOwner() && (target->getOwner()->name == issuer->name)) {
        throw InvalidOrderException(issuer->name + " attempts to bomb himself! What an idiot.");
    }
}

/**
 * Execute the bomb order
 */
void BombOrder::execute() {
    validate();
    this->target->setArmies(this->target->getArmies() / 2);
    this->Notify(this);
}

/**
 * Bomb order copy constructor
 * @param other The to-be-copied object
 */
BombOrder::BombOrder(const BombOrder &other) : Order(other.issuer, other.name), target(other.target) {}

/**
 * Bomb order destructor
 */
BombOrder::~BombOrder() = default;

// ------------------ BlockadeOrder ------------------------
/**
 * Blockade order constructor
 * @param issuer  The player issuing the blockade order
 * @param target The target territory of the blockade order
 */
BlockadeOrder::BlockadeOrder(Player *issuer, Territory *target)
        : Order(issuer, "Blockade"),
          target(target) {}

/**
 * Description of the blockade order
 * @return String with the description
 */
std::string BlockadeOrder::description() {
    return issuer->name + " blockades " + target->toString();
}

/**
 * Validate if the blockade order can be executed
 */
void BlockadeOrder::validate() {
    if (target->getOwner() && (target->getOwner() != issuer)) {
        throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
    }
}

/**
 * Execute the blockade order
 */
void BlockadeOrder::execute() {
    validate();
    this->target->setArmies(this->target->getArmies() * 3);
    this->Notify(this);
}

/**
 * Blockade order copy constructor
 * @param other The to-be-copied object
 */
BlockadeOrder::BlockadeOrder(const BlockadeOrder &other) : Order(other.issuer, other.name), target(other.target) {}

/**
 * Blockade order destructor
 */
BlockadeOrder::~BlockadeOrder() = default;

// ------------------ AirliftOrder ------------------------
/**
 * Airlift order constructor
 * @param issuer the player issuing the airlift order
 * @param armies the number of armies to airlift
 * @param source The source territory where the armies will be taken from
 * @param target The target territory where the armies will land
 */
AirliftOrder::AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target)
        : Order(issuer, "Airlift"),
          armies(armies), source(source), target(target) {}

/**
 * Description of the airlift order
 * @return String with the description
 */
std::string AirliftOrder::description() {
    return issuer->name + " airlifts " + std::to_string(armies) + " armies from " + source->toString() +
           " to " +
           target->toString();
}

/**
 * Validate if the airlift order can be executed
 */
void AirliftOrder::validate() {
    if (source->getArmies() - armies < 0) {
        throw InvalidOrderException(
                issuer->name + "'s source territory (" + source->getName() + ") does not have " + std::to_string(armies)
                + " number of army members.");
    } else if (target->getOwner() && (target->getOwner() != issuer)) {
        throw InvalidOrderException(issuer->name + " does not own territory " + target->getName());
    }
}

/**
 * Execute the airlift order
 */
void AirliftOrder::execute() {
    validate();
    int sourceArmies = this->source->getArmies();
    int targetArmies = this->target->getArmies();
    this->source->setArmies(sourceArmies - armies);
    this->target->setArmies(targetArmies + armies);
    this->Notify(this);
}

/**
 * Copy constructor for an airlfit order
 * @param other The to-be-copied object
 */
AirliftOrder::AirliftOrder(const AirliftOrder &other) : Order(other.issuer, other.name), target(other.target),
                                                        source(other.source), armies(other.armies) {}

/**
 * Airlift order destructor
 */
AirliftOrder::~AirliftOrder() = default;

// ------------------ NegotiateOrder ------------------------
/**
 * Negotiate order constructor
 * @param issuer The player issuing the negotiate order
 * @param target The target player to negotiate with
 */
NegotiateOrder::NegotiateOrder(Player *issuer, const Player *target)
        : Order(issuer, "Negotiate"),
          target(target) {}

/**
 * Description of the negotiate order
 * @return String with the description
 */
std::string NegotiateOrder::description() {
    return issuer->name + " negotiates with " + target->name;
}

/**
 * Negotiate order copy constructor
 * @param o The to-be-copied object
 */
NegotiateOrder::NegotiateOrder(const NegotiateOrder &o) : Order(o.issuer, o.name), target(o.target) {}

/**
 * Negotiate order destructor
 */
NegotiateOrder::~NegotiateOrder() = default;

// ------------------ OrderList ------------------------

/**
 * Push an order to the list of orders
 * @param order The order to be pushed
 */
void OrderList::push(Order *order) {
    this->orders.push_back(order);
    this->Notify(this);
}

/**
 * Pop an order from the order list
 * @return the order that has been removed
 */
Order *OrderList::pop() {
    if (this->orders.empty())
        return nullptr;
    auto order = this->orders.front();
    this->orders.erase(this->orders.begin());
    return order;
}

/**
 * Get an order at a certain index within the order list
 * @param index The location
 * @return The order at that index
 */
Order *OrderList::get(int index) {
    return this->orders[index];
}

/**
 * Remove an order from the order list at a specific index
 * @param index The index where the order will be removed
 */
void OrderList::remove(int index) {
    auto order = orders[index];
    this->orders.erase(this->orders.begin() + index);
    delete order;
}

/**
 * Move the location of two orders within the order list
 * @param a The index of the first order
 * @param b The index of the second order
 */
void OrderList::move(int a, int b) {
    if ((0 <= a && a < this->orders.size()) && (0 <= b && b < this->orders.size())) {
        std::swap(this->orders[a], this->orders[b]);
    } else {
        throw runtime_error("Index out of bounds!");
    }
}

/**
 * Execute all orders within the order list
 */
void OrderList::executeOrders() {
    while (!this->orders.empty()) {
        auto order = pop();
        order->execute();
        delete order;
    }
}

/**
 * Get the size of the list of orders to be executed
 * @return The size of the list of orders to be executed
 */
int OrderList::getOrdersSize() {
    return orders.size();
}

/**
 * Function to print to console the content's of the order list
 * @param os The OSStream we will be appending to
 * @param hand  The order list we will be examining
 * @return A string containing the content's of the order list
 */
std::ostream &operator<<(std::ostream &os, const OrderList &orderList) {
    auto i = 1;
    auto divider = "--------------------------------------------------------------------------------";

    std::cout << divider << "\n" << std::left << std::setw(3) << "Id | " << std::left << std::setw(10) << "Name"
              << "| Description"
              << "\n" << divider << std::endl;

    for (auto order: orderList.orders) {
        std::cout << std::left << std::setw(3) << std::to_string(i) << "| " << std::left << std::setw(10)
                  << order->name
                  << "| " + order->description()
                  << std::endl;
        ++i;
    }

    std::cout << divider << std::endl;
    return os;
}

/**
 * Order list copy constructor
 * @param o The to-be-copied object
 */
OrderList::OrderList(const OrderList &o) {
    orders = o.orders;
    this->Attach(LogObserver::instance());
}

/**
 * Order list default constructor
 */
OrderList::OrderList() {
    this->Attach(LogObserver::instance());
}

/**
 * Destructor for OrderList
 */
OrderList::~OrderList() {
    this->Detach(LogObserver::instance());
}

// ------------------ Exception ------------------------
/**
 * Exception for invalid order
 * @param arg The text that will be printed on error
 */
InvalidOrderException::InvalidOrderException(const std::string &arg) : runtime_error(arg) {}

