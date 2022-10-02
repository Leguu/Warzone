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

class Order {
public:
  const std::string name;
  Player *issuer;
  Order(Player *issuer, std::string name);

  /// Throws InvalidOrderException if the order no longer makes sense (due to previous orders)
  virtual void validate() noexcept(false) = 0;

  virtual std::string description() = 0;

  virtual void execute() = 0;

  friend std::ostream &operator<<(std::ostream &os, Order &order);

  virtual ~Order();
};

class DeployOrder : public Order {
public:
  explicit DeployOrder(Player *issuer, int reinforcements, Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~DeployOrder() override;

private:
  const int reinforcements;
  Territory *target;
};

class AdvanceOrder : public Order {
public:
  AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target);

  void validate() override {};

  void execute() override {};

  std::string description() override;

  ~AdvanceOrder() override;

private:
  const int armies;
  Territory *source;
  Territory *target;
};

class BombOrder : public Order {
public:
  explicit BombOrder(Player *issuer, Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~BombOrder() override;

private:
  Territory *target;
};

class BlockadeOrder : public Order {
public:
  explicit BlockadeOrder(Player *issuer, Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~BlockadeOrder() override;

private:
  Territory *target;
};

class AirliftOrder : public Order {
public:
  AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~AirliftOrder() override;

private:
  const int armies;
  Territory *source;
  Territory *target;
};

class NegotiateOrder : public Order {
public:
  explicit NegotiateOrder(Player *issuer, const Player *target);

  void validate() override {};

  void execute() override {};

  std::string description() override;

  ~NegotiateOrder() override;

private:
  const Player *const target;
};

class OrderList {
public:
  void remove(int index);

  void move(int a, int b);

  Order *get(int index);

  Order *pop();

  void push(Order *order);

  void executeOrders();

  int getOrdersSize();

  friend std::ostream &operator<<(std::ostream &os, const OrderList &orderList);

private:
  std::vector<Order *> orders = {};
};

#endif //WARZONE_ORDER_H
