//
// Created by Legu on 2022-09-13.
//

#include "Order.h"
#include "../GameEngine.h"

#include <utility>

Order::Order(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}

std::ostream &operator<<(std::ostream &os, const Order &order) {
    os << order.name << ": " << order.description;
    return os;
}

void BombOrder::execute() {
    this->target->armies = this->target->armies / 2;
}

bool BombOrder::valid() {
    if (!this->target) {
        return false;
    }
    return true;
}
