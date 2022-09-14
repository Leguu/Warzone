//
// Created by Legu on 2022-09-13.
//

#ifndef WARZONE_ORDER_H
#define WARZONE_ORDER_H


#include <vector>
#include <string>
#include <ostream>
#include "../Map/Map.h"

class Order {
public:
    const std::string name;
    const std::string description;

    Order(std::string name, std::string description);

    virtual bool valid() = 0;

    virtual void execute() = 0;

    friend std::ostream &operator<<(std::ostream &os, const Order &order);
};

class DeployOrder : public Order {
public:
    inline DeployOrder(int reinforcements, Territory *target)
            : Order("Deploy", "Moving " + std::to_string(reinforcements) + " armies to " + target->name),
              reinforcements(reinforcements), target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    const int reinforcements;
    Territory *target;
};

class AdvanceOrder : public Order {
public:
    inline AdvanceOrder(int armies, Territory *source, Territory *target)
            : Order("Advance",
                    "Advancing " + std::to_string(armies) + " armies from " + source->name + " to " + target->name),
              armies(armies), source(source), target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    const int armies;
    Territory *source;
    Territory *target;
};

class BombOrder : public Order {
public:
    inline explicit BombOrder(Territory *target)
            : Order("Bomb", "Bombing " + target->name),
              target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    Territory *target;
};

class BlockadeOrder : public Order {
public:
    inline explicit BlockadeOrder(Territory *target)
            : Order("Blockade", "Blockading " + target->name),
              target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    Territory *target;
};

class AirliftOrder : public Order {
public:
    inline AirliftOrder(int armies, Territory *source, Territory *target)
            : Order("Airlift",
                    "Airlifting " + std::to_string(armies) + " armies from " + source->name + " to " + target->name),
              armies(armies), source(source), target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    const int armies;
    Territory *const source;
    Territory *const target;
};

class NegotiateOrder : public Order {
public:
    inline explicit NegotiateOrder(const Player *target)
            : Order("Negotiate", "Negotiating with " + target->name),
              target(target) {}

private:
    bool valid() override;

    void execute() override;

private:
    const Player *const target;
};

class OrdersList {
public:
    void remove(int index);

    void move(int a, int b);

private:
    // May be better as a queue?
    std::vector<Order> orders;
};


#endif //WARZONE_ORDER_H
