//
// Created by Pin Ren Toh on 23/9/23.
//

#include "Order.h"
#include "Limit.h"
#include <ctime>

Order::Order(int id, float price, int quantity, bool isBuyOrder, time_t time) {
    this->id = id;
    this->price = price;
    this->quantity = quantity;
    this->isBuyOrder = isBuyOrder;
    this->time = time;
}

int Order::getId() const {
    return id;
}

int Order::getPrice() const {
    return price;
}

int Order::getQuantity() const {
    return quantity;
}

bool Order::isBuy() const {
    return isBuyOrder;
}

int Order::getTime() const {
    return time;
}

Order *Order::getNextOrder() const {
    return nextOrder;
}

Order *Order::getPrevOrder() const {
    return prevOrder;
}

Limit *Order::getParentLimit() const {
    return parentLimit;
}

void Order::setNextOrder(Order *nextOrder) {
    this->nextOrder = nextOrder;
}

void Order::setPrevOrder(Order *prevOrder) {
    this->prevOrder = prevOrder;
}