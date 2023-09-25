//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_ORDER_H
#define ORDER_BOOK_ORDER_H

#include "Limit.h"
#include <ctime>

class Order {
public:
    Order(int id, float price, int quantity, bool isBuyOrder, time_t time);

    int getId() const;

    int getPrice() const;

    int getQuantity() const;

    bool isBuy() const;

    int getTime() const;

    Order *getNextOrder() const;

    Order *getPrevOrder() const;

    Limit *getParentLimit() const;

    void setNextOrder(Order *nextOrder);

    void setPrevOrder(Order *prevOrder);

    int id;

    int price;

    int quantity;

    bool isBuyOrder;

    time_t time;

    Order *nextOrder;

    Order *prevOrder;

    Limit *parentLimit;

    void decreaseQuantity(int quantity);
};


#endif //ORDER_BOOK_ORDER_H
