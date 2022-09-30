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
  virtual void validate() noexcept(false) = 0;

  virtual void execute() = 0;

  friend std::ostream &operator<<(std::ostream &os, const Order &order);

  virtual std::string_view getName() const = 0;

  virtual ~Order();
};

class DeployOrder : public Order {
public:
  explicit DeployOrder(Player *issuer, int reinforcements, Territory *target);

  void validate() override;

  void execute() override;

  [[nodiscard]] std::string_view getName() const override {
	return "DeployOrder";
  }

  ~DeployOrder() override;

private:
  const int reinforcements;
  Territory *target;
};

class AdvanceOrder : public Order {
public:
  AdvanceOrder(Player *issuer, int armies, Territory *source, Territory *target);

  void validate() override;

  void execute() override;

  [[nodiscard]] std::string_view getName() const override {
	return "AdvanceOrder";
  }

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

  [[nodiscard]] std::string_view getName() const override {
	return "BombOrder";
  }

  ~BombOrder() override;

private:
  // TODO All these Territory * could maybe be references?
  // They should definitely not be nullable.
  Territory *target;
};

class BlockadeOrder : public Order {
public:
  explicit BlockadeOrder(Player *issuer, Territory *target);

  void validate() override;

  void execute() override;

  [[nodiscard]] std::string_view getName() const override {
	return "BlockadeOrder";
  }

  ~BlockadeOrder() override;

private:
  Territory *target;
};

class AirliftOrder : public Order {
public:
  AirliftOrder(Player *issuer, int armies, Territory *source, Territory *target);

  void validate() override;

  void execute() override;

  [[nodiscard]] std::string_view getName() const override {
	return "AirliftOrder";
  }

  ~AirliftOrder() override;

private:
  const int armies;
  Territory *source;
  Territory *target;
};

class NegotiateOrder : public Order {
public:
  explicit NegotiateOrder(Player *issuer, const Player *target);

  void validate() override;

  void execute() override;

  [[nodiscard]] std::string_view getName() const override {
	return "NegotiateOrder";
  }

private:
  const Player *const target;
};

class OrderList {
public:
  void remove(int index);

  void move(int a, int b);

  Order *pop();

  void push(Order *order);

  void executeOrders();

  friend std::ostream &operator<<(std::ostream &os, const OrderList &orderList);

private:
  std::vector<Order *> orders = {};
};

#endif //WARZONE_ORDER_H
