//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_ORDER_H
#define ORDER_BOOK_ORDER_H

#include "Limit.h"
#include <ctime>

/**
 * Class representing an order in the order book.
 */
class Order {
private:
    /**
     * ID of the order.
     */
    int id;

    /**
     * Price of the order.
     */
    float price;

    /**
     * Quantity of the order.
     */
    int quantity;

    /**
     * Boolean indicating if the order is a buy order.
     */
    bool isBuyOrder;

    /**
     * Time the order was placed.
     */
    time_t time;

    /**
     * Pointer to the next order in the linked list.
     */
    Order *nextOrder;

    /**
     * Pointer to the previous order in the linked list.
     */
    Order *prevOrder;

    /**
     * Pointer to the parent limit of the order.
     */
    Limit *parentLimit;

public:
    /**
     * Constructor for Order.
     *
     * @param id ID of the order
     * @param price Price of the order
     * @param quantity Quantity of the order
     * @param isBuyOrder Boolean indicating if the order is a buy order
     * @param time Time the order was placed
     */
    Order(int id, float price, int quantity, bool isBuyOrder, time_t time);

    /**
     * Getter for the ID of the order.
     *
     * @return ID of the order
     */
    int getId() const;

    /**
     * Getter for the price of the order.
     *
     * @return Price of the order
     */
    float getPrice() const;

    /**
     * Getter for the quantity of the order.
     *
     * @return Quantity of the order
     */
    int getQuantity() const;

    /**
     * Getter for the boolean indicating if the order is a buy order.
     *
     * @return Whether the order is a buy order
     */
    bool isBuy() const;

    /**
     * Getter for the time the order was placed.
     *
     * @return Time the order was placed
     */
    time_t getTime() const;

    /**
     * Getter for the next order in the linked list.
     *
     * @return Next order in the linked list
     */
    Order *getNextOrder() const;

    /**
     * Getter for the previous order in the linked list.
     *
     * @return Previous order in the linked list
     */
    Order *getPrevOrder() const;

    /**
     * Getter for the parent limit of the order.
     *
     * @return Parent limit of the order
     */
    Limit *getParentLimit() const;

    /**
     * Setter for the next order in the linked list.
     *
     * @param nextOrder Order to set as the next order in the linked list
     */
    void setNextOrder(Order *nextOrder);

    /**
     * Setter for the previous order in the linked list.
     *
     * @param prevOrder Order to set as the previous order in the linked list
     */
    void setPrevOrder(Order *prevOrder);

    /**
     * Setter for the parent limit of the order.
     *
     * @param parentLimit Limit to set as the parent limit of the order
     */
    void setParentLimit(Limit *parentLimit);

    /**
     * Decreases the quantity of the order by the given quantity.
     *
     * @param quantity Quantity to decrease the order quantity by
     */
    void decreaseQuantity(int amount);
};


#endif //ORDER_BOOK_ORDER_H
