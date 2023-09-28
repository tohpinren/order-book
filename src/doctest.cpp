#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"

TEST_CASE("Order") {
    // Create subtest
    SUBCASE("Create order") {
        // Create Order
        Order *order = new Order(1, 100, 10, true, 0);
        CHECK(order->getId() == 1);
        CHECK(order->getPrice() == 100);
        CHECK(order->getQuantity() == 10);
        CHECK(order->isBuy() == true);
        CHECK(order->getTime() == 0);
    }
}

TEST_CASE("Limit") {
    SUBCASE("Create limit") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        CHECK(limit->getPrice() == 100);
        CHECK(limit->getSize() == 0);
        CHECK(limit->getTotalVolume() == 0);
        CHECK(limit->getParent() == nullptr);
        CHECK(limit->getLeftChild() == nullptr);
        CHECK(limit->getRightChild() == nullptr);
        CHECK(limit->getHeadOrder() == nullptr);
        CHECK(limit->getTailOrder() == nullptr);
        CHECK(limit->getHeight() == 0);
    }
}

TEST_CASE("OrderBook") {
    OrderBook *orderBook = new OrderBook();
}