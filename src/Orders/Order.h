#ifndef WARZONE_ORDER_H
#define WARZONE_ORDER_H

class OrderList;
class Order;

#include "../Logging/LoggingObserver.h"
#include "../Player/Player.h"
#include <list>
#include <ostream>
#include <queue>
#include <string>
#include <vector>
using std::runtime_error;

/// Thrown when an order can not be executed due to invalid state
class InvalidOrderException : public std::runtime_error {
public:
  explicit InvalidOrderException(const std::string &arg);
};

class Order : public ILoggable, public Subject {
public:
  const std::string name;
  Player *issuer;
  Order(Player *issuer, std::string name);
  std::string stringToLog() override;

  /// Throws InvalidOrderException if the order no longer makes sense (due to
  /// previous orders)
  virtual void validate() noexcept(false) = 0;

  virtual std::string description() = 0;

  virtual void execute() = 0;

  friend std::ostream &operator<<(std::ostream &os, Order &order);

  virtual ~Order();
};

class DeployOrder : public Order {
public:
  DeployOrder(Player *issuer, int reinforcements, Territory *target);

  DeployOrder(const DeployOrder &other);

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
  AdvanceOrder(Player *issuer, int armies, Territory *source,
			   Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~AdvanceOrder() override;

  AdvanceOrder(const AdvanceOrder &other);

  Territory *getTarget();

private:
  int armies;
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

  BombOrder(const BombOrder &other);

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

  BlockadeOrder(const BlockadeOrder &other);

private:
  Territory *target;
};

class AirliftOrder : public Order {
public:
  AirliftOrder(Player *issuer, int armies, Territory *source,
			   Territory *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~AirliftOrder() override;

  AirliftOrder(const AirliftOrder &other);

private:
  const int armies;
  Territory *source;
  Territory *target;
};

class NegotiateOrder : public Order {
public:
  explicit NegotiateOrder(Player *issuer, Player *target);

  void validate() override;

  void execute() override;

  std::string description() override;

  ~NegotiateOrder() override;

  NegotiateOrder(const NegotiateOrder &o);

private:
  Player *target;
};

class OrderList : public ILoggable, public Subject {
public:
  void remove(int index);

  void move(int a, int b);

  Order *get(int index);

  Order *pop();

  void push(Order *order);

  void executeOrders();

  int getOrdersSize();

  std::string stringToLog() override;

  friend std::ostream &operator<<(std::ostream &os, const OrderList &orderList);

  OrderList();
  OrderList(const OrderList &o);
  ~OrderList();

private:
  std::vector<Order *> orders = {};
  friend void testLoggingObserver();
};

void testOrdersLists();

#endif // WARZONE_ORDER_H
