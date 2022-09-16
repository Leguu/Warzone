#ifndef WARZONE_ORDER_H
#define WARZONE_ORDER_H

class OrderList;

class Order;

#include <vector>
#include <string>
#include <ostream>
#include <queue>
#include <list>
#include "../Player/Player.h"

/// Thrown when an order can not be executed due to invalid state
class InvalidOrderException : public std::runtime_error {
public:
    explicit InvalidOrderException(const std::string &arg);
};

// Todo implement all orders
// TODO implement a way to issue orders without card... maybe an order manager? yeah... order manager owned by gameengine (should be in GameEngine.h)
class Order {
public:
    const std::string name;
    // TODO: maybe this should be a function... maybe the number of people changes between when an order is created and it is executed
    const std::string description;
    Player *issuer;

    Order(Player *issuer, std::string name, std::string description);

    /// Throws InvalidOrderException if the order no longer makes sense (due to previous orders)
    virtual void execute() noexcept(false) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Order &order);

    virtual ~Order();
};

class DeployOrder : public Order {
public:
    DeployOrder(Player *issuer, int reinforcements, Territory *target);

    inline void execute() override {};

    ~DeployOrder() override;

private:
    const int reinforcements;
    Territory *target;
};

class AdvanceOrder : public Order {
public:
    AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target);

    inline void execute() override {};

    ~AdvanceOrder() override;

private:
    const int armies;
    Territory *source;
    Territory *target;
};

class BombOrder : public Order {
public:
    explicit BombOrder(Player *issuer, Territory *target);

    void execute() override;

    ~BombOrder() override;

private:
    // TODO All these Territory * could maybe be references?
    // They should definitely not be nullable.
    Territory *target;
};

class BlockadeOrder : public Order {
public:
    explicit BlockadeOrder(Player *issuer, Territory *target);

private:
    inline void execute() override {};

private:
    Territory *target;
};

class AirliftOrder : public Order {
public:
    AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target);

    inline void execute() override {};

private:
    const int armies;
    Territory *source;
    Territory *target;
};

class NegotiateOrder : public Order {
public:
    explicit NegotiateOrder(Player *issuer, const Player *target);

    inline void execute() override {};

private:
    const Player *const target;
};

class OrderList {
public:
    void remove(int index);

    void move(int a, int b);

    std::unique_ptr<Order> pop();

    void push(std::unique_ptr<Order> order);

private:
    std::list<std::unique_ptr<Order>> orders = {};
};


#endif //WARZONE_ORDER_H
