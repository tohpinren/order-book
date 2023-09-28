//
// Created by Pin Ren Toh on 23/9/23.
//

#include "Order.h"
#include "Limit.h"

/**
 * Constructor for Order.
 *
 * @param id ID of the order
 * @param price Price of the order
 * @param quantity Quantity of the order
 * @param isBuyOrder Boolean indicating if the order is a buy order
 * @param time Time the order was placed
 */
Order::Order(int id, float price, int quantity, bool isBuyOrder, time_t time) {
    this->id = id;
    this->price = price;
    this->quantity = quantity;
    this->isBuyOrder = isBuyOrder;
    this->time = time;
    this->nextOrder = nullptr;
    this->prevOrder = nullptr;
    this->parentLimit = nullptr;
}

/**
 * Getter for ID of the order.
 *
 * @return ID of the order
 */
int Order::getId() const {
    return id;
}

/**
 * Getter for price of the order.
 *
 * @return Price of the order
 */
float Order::getPrice() const {
    return price;
}

/**
 * Getter for quantity of the order.
 *
 * @return Quantity of the order
 */
int Order::getQuantity() const {
    return quantity;
}

/**
 * Getter for boolean indicating if the order is a buy order.
 *
 * @return Whether the order is a buy order
 */
bool Order::isBuy() const {
    return isBuyOrder;
}

/**
 * Getter for time the order was placed.
 *
 * @return Time the order was placed
 */
time_t Order::getTime() const {
    return time;
}

/**
 * Getter for the next order in the linked list.
 *
 * @return Next order in the linked list
 */
Order *Order::getNextOrder() const {
    return nextOrder;
}

/**
 * Getter for the previous order in the linked list.
 *
 * @return Previous order in the linked list
 */
Order *Order::getPrevOrder() const {
    return prevOrder;
}

/**
 * Getter for the parent limit of the order.
 *
 * @return Parent limit of the order
 */
Limit *Order::getParentLimit() const {
    return parentLimit;
}

/**
 * Setter for the next order in the linked list.
 *
 * @param nextOrder Order to set as the next order in the linked list
 */
void Order::setNextOrder(Order *newNextOrder) {
    this->nextOrder = newNextOrder;
}

/**
 * Setter for the previous order in the linked list.
 *
 * @param prevOrder Order to set as the previous order in the linked list
 */
void Order::setPrevOrder(Order *newPrevOrder) {
    this->prevOrder = newPrevOrder;
}

/**
 * Setter for the parent limit of the order.
 *
 * @param quantity Quantity to decrease the order quantity by
 */
void Order::decreaseQuantity(int amount) {
    this->quantity -= amount;
    this->getParentLimit()->decreaseVolume(amount);
}

/**
 * Setter for the parent limit of the order.
 *
 * @param parentLimit Limit to set as the parent limit of the order
 */
void Order::setParentLimit(Limit *newParentLimit) {
    this->parentLimit = newParentLimit;
}
