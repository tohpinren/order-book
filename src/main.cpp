//
// Created by Pin Ren Toh on 24/9/23.
//

#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"
#include <iostream>

int main() {
    OrderBook *orderBook = new OrderBook();

    orderBook->addOrder(100, 10, true);
    orderBook->addOrder(90, 10, true);
    orderBook->addOrder(110, 10, true);

    orderBook->addOrder(100, 10, false);
    orderBook->executeOrder();

    return 0;
}