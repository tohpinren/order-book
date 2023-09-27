#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"

TEST_CASE("Order") {
    Order *order = new Order(1, 100, 10, true, 0);
    CHECK(order->getId() == 1);
    CHECK(order->getPrice() == 100);
    CHECK(order->getQuantity() == 10);
    CHECK(order->isBuy() == true);
    CHECK(order->getTime() == 0);
}

TEST_CASE("Limit") {

}

TEST_CASE("OrderBook") {

}