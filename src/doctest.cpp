#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"
#include <queue>

TEST_CASE("Order") {
    SUBCASE("Create order") {
        // Create Order
        Order *order = new Order(1, 100, 10, true, 0);
        CHECK(order->getId() == 1);
        CHECK(order->getPrice() == 100);
        CHECK(order->getQuantity() == 10);
        CHECK(order->isBuy() == true);
        CHECK(order->getTime() == 0);
    }

    SUBCASE("Set next and prev orders") {
        Order *order1 = new Order(1, 100, 10, true, 0);
        Order *order2 = new Order(2, 100, 10, true, 0);
        order1->setNextOrder(order2);
        order2->setPrevOrder(order1);
        CHECK(order1->getNextOrder() == order2);
        CHECK(order2->getPrevOrder() == order1);
    }

    SUBCASE("Set parent limit and decrease quantity") {
        Order *order = new Order(1, 100, 10, true, 0);
        Limit *limit = new Limit(100, true, nullptr);
        order->setParentLimit(limit);
        CHECK(order->getParentLimit() == limit);
        CHECK(order->getQuantity() == 10);
        order->decreaseQuantity(5);
        CHECK(order->getQuantity() == 5);
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

    SUBCASE("Increase size decrease size") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        limit->increaseSize(10);
        CHECK(limit->getSize() == 10);
        limit->decreaseSize(10);
        CHECK(limit->getSize() == 0);
    }

    SUBCASE("Increase volume decrease volume") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        limit->increaseVolume(10);
        CHECK(limit->getTotalVolume() == 10);
        limit->decreaseVolume(10);
        CHECK(limit->getTotalVolume() == 0);
    }

    SUBCASE("Set height") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        limit->setHeight(10);
        CHECK(limit->getHeight() == 10);
    }

    SUBCASE("Get inside order within limit") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        Order *order1 = new Order(1, 100, 10, true, 0);
        Order *order2 = new Order(2, 100, 10, true, 0);
        Order *order3 = new Order(3, 100, 10, true, 0);
        limit->addOrder(order1);
        limit->addOrder(order2);
        limit->addOrder(order3);
        CHECK(limit->getNextInsideOrder() == order1);
    }

    SUBCASE("Set parent, left child, right child") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit1 = new Limit(100, true, orderBook);
        Limit *limit2 = new Limit(90, true, orderBook);
        Limit *limit3 = new Limit(110, true, orderBook);
        limit2->setParent(limit1);
        limit1->setLeftChild(limit2);
        limit3->setParent(limit1);
        limit1->setRightChild(limit3);
        CHECK(limit2->getParent() == limit1);
        CHECK(limit3->getParent() == limit1);
        CHECK(limit1->getLeftChild() == limit2);
        CHECK(limit1->getRightChild() == limit3);
    }

    SUBCASE("Set head and tail order") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        Order *order1 = new Order(1, 100, 10, true, 0);
        Order *order2 = new Order(2, 100, 10, true, 0);
        limit->setHeadOrder(order1);
        limit->setTailOrder(order2);
        CHECK(limit->getHeadOrder() == order1);
        CHECK(limit->getTailOrder() == order2);
    }

    SUBCASE("Add order and remove order") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit = new Limit(100, true, orderBook);
        Order *order1 = new Order(1, 100, 10, true, 0);
        Order *order2 = new Order(2, 100, 10, true, 0);
        Order *order3 = new Order(3, 100, 10, true, 0);
        limit->addOrder(order1);
        limit->addOrder(order2);
        limit->addOrder(order3);
        CHECK(limit->getSize() == 3);
        CHECK(limit->getTotalVolume() == 30);
        CHECK(limit->getHeadOrder() == order1);
        CHECK(limit->getTailOrder() == order3);
        limit->removeOrder(order2);
        CHECK(limit->getSize() == 2);
        CHECK(limit->getTotalVolume() == 20);
        CHECK(limit->getHeadOrder() == order1);
        CHECK(limit->getTailOrder() == order3);
        limit->removeOrder(order1);
        CHECK(limit->getSize() == 1);
        CHECK(limit->getTotalVolume() == 10);
        CHECK(limit->getHeadOrder() == order3);
        CHECK(limit->getTailOrder() == order3);
        limit->removeOrder(order3);
        CHECK(limit->getSize() == 0);
        CHECK(limit->getTotalVolume() == 0);
        CHECK(limit->getHeadOrder() == nullptr);
        CHECK(limit->getTailOrder() == nullptr);
    }

    SUBCASE("Insert limit") {
        OrderBook *orderBook = new OrderBook();
        Limit *limit1 = new Limit(100, true, orderBook);
        Limit *limit2 = new Limit(90, true, orderBook);
        Limit *limit3 = new Limit(110, true, orderBook);
        limit1->insertLimit(limit2);
        limit1->insertLimit(limit3);
        CHECK(limit1->getLeftChild() == limit2);
        CHECK(limit1->getRightChild() == limit3);
        CHECK(limit2->getParent() == limit1);
        CHECK(limit3->getParent() == limit1);
    }
}

TEST_CASE("OrderBook") {
    SUBCASE("Create order book") {
        OrderBook *orderBook = new OrderBook();
        CHECK(orderBook->getBuyTree() == nullptr);
        CHECK(orderBook->getSellTree() == nullptr);
    }

    SUBCASE("Set buy and sell tree") {
        OrderBook *orderBook = new OrderBook();
        Limit *sellLimit = new Limit(100, false, orderBook);
        Limit *buyLimit = new Limit(90, true, orderBook);
        orderBook->setBuyTree(buyLimit);
        orderBook->setSellTree(sellLimit);
        CHECK(orderBook->getBuyTree() == buyLimit);
        CHECK(orderBook->getSellTree() == sellLimit);
    }

    SUBCASE("Add order") {
        OrderBook *orderBook = new OrderBook();
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(200, 10, true);
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(400, 10, true);
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(300, 10, true);
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(1000, 10, true);
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(500, 10, true);
        orderBook->addOrder(5000, 10, false);
        orderBook->addOrder(3000, 10, false);
        orderBook->addOrder(4000, 10, false);
        orderBook->addOrder(1000, 10, false);
        orderBook->addOrder(900, 10, false);
        orderBook->addOrder(700, 10, false);
        orderBook->addOrder(600, 10, false);
        orderBook->addOrder(1500, 10, false);
        orderBook->addOrder(2000, 10, false);
        orderBook->addOrder(800, 10, false);

        // Bfs to check size of buy tree
        int totalBuySize = 0;
        int totalBuyVolume = 0;
        Limit *buyTree = orderBook->getBuyTree();
        std::queue<Limit *> queue;
        queue.push(buyTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalBuySize += limit->getSize();
            totalBuyVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalBuySize == 10);
        CHECK(totalBuyVolume == 100);

        // Bfs to check size of sell tree
        int totalSellSize = 0;
        int totalSellVolume = 0;
        Limit *sellTree = orderBook->getSellTree();
        queue.push(sellTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalSellSize += limit->getSize();
            totalSellVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalSellSize == 10);
        CHECK(totalSellVolume == 100);
    }

    SUBCASE("Cancel order") {
        OrderBook *orderBook = new OrderBook();
        Order *buyOrder1 = orderBook->addOrder(100, 10, true);
        orderBook->addOrder(200, 10, true);
        orderBook->addOrder(300, 10, true);
        Order *buyOrder4 = orderBook->addOrder(400, 10, true);
        orderBook->addOrder(500, 10, true);
        orderBook->addOrder(1000, 10, false);
        Order *sellOrder2 = orderBook->addOrder(900, 10, false);
        orderBook->addOrder(800, 10, false);
        orderBook->addOrder(700, 10, false);
        Order *sellOrder5 = orderBook->addOrder(600, 10, false);

        // Bfs to check size of buy tree
        int totalBuySize = 0;
        int totalBuyVolume = 0;
        Limit *buyTree = orderBook->getBuyTree();
        std::queue<Limit *> queue;
        queue.push(buyTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalBuySize += limit->getSize();
            totalBuyVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalBuySize == 5);
        CHECK(totalBuyVolume == 50);

        // Bfs to check size of sell tree
        int totalSellSize = 0;
        int totalSellVolume = 0;
        Limit *sellTree = orderBook->getSellTree();
        queue.push(sellTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalSellSize += limit->getSize();
            totalSellVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalSellSize == 5);
        CHECK(totalSellVolume == 50);

        orderBook->cancelOrder(buyOrder1);
        orderBook->cancelOrder(buyOrder4);
        orderBook->cancelOrder(sellOrder2);
        orderBook->cancelOrder(sellOrder5);

        // Bfs to check size of buy tree
        totalBuySize = 0;
        totalBuyVolume = 0;
        buyTree = orderBook->getBuyTree();
        queue.push(buyTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalBuySize += limit->getSize();
            totalBuyVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalBuySize == 3);
        CHECK(totalBuyVolume == 30);

        // Bfs to check size of sell tree
        totalSellSize = 0;
        totalSellVolume = 0;
        sellTree = orderBook->getSellTree();
        queue.push(sellTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalSellSize += limit->getSize();
            totalSellVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalSellSize == 3);
        CHECK(totalSellVolume == 30);
    }

    SUBCASE("Execute order") {
        OrderBook *orderBook = new OrderBook();
        Order *buyOrder1 = orderBook->addOrder(100, 10, true);
        Order *buyOrder2 = orderBook->addOrder(200, 10, true);
        Order *buyOrder3 = orderBook->addOrder(300, 10, true);
        Order *buyOrder4 = orderBook->addOrder(400, 10, true);
        Order *buyOrder5 = orderBook->addOrder(500, 10, true);
        Order *sellOrder1 = orderBook->addOrder(1000, 10, false);
        Order *sellOrder2 = orderBook->addOrder(900, 10, false);
        Order *sellOrder3 = orderBook->addOrder(800, 10, false);
        Order *sellOrder4 = orderBook->addOrder(700, 10, false);
        Order *sellOrder5 = orderBook->addOrder(400, 10, false);

        // Bfs to check size of buy tree
        int totalBuySize = 0;
        int totalBuyVolume = 0;
        Limit *buyTree = orderBook->getBuyTree();
        std::queue<Limit *> queue;
        queue.push(buyTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalBuySize += limit->getSize();
            totalBuyVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalBuySize == 5);
        CHECK(totalBuyVolume == 50);

        // Bfs to check size of sell tree
        int totalSellSize = 0;
        int totalSellVolume = 0;
        Limit *sellTree = orderBook->getSellTree();
        queue.push(sellTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalSellSize += limit->getSize();
            totalSellVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalSellSize == 5);
        CHECK(totalSellVolume == 50);

        // Redirect std::cout to a stringstream
        std::stringstream capturedOutput;
        std::streambuf* originalOutputBuffer = std::cout.rdbuf();
        std::cout.rdbuf(capturedOutput.rdbuf());

        orderBook->executeOrder();

        // Restore the original std::cout buffer
        std::cout.rdbuf(originalOutputBuffer);

        // Get the captured output
        std::string output = capturedOutput.str();

        // Check the captured output against the expected output
        CHECK(output == "Executed buy order at 500 and sell order at 400\n"
                        "Profit: 1000\n");

        // Bfs to check size of buy tree
        totalBuySize = 0;
        totalBuyVolume = 0;
        buyTree = orderBook->getBuyTree();
        queue.push(buyTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalBuySize += limit->getSize();
            totalBuyVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalBuySize == 4);
        CHECK(totalBuyVolume == 40);

        // Bfs to check size of sell tree
        totalSellSize = 0;
        totalSellVolume = 0;
        sellTree = orderBook->getSellTree();
        queue.push(sellTree);
        while (!queue.empty()) {
            Limit *limit = queue.front();
            queue.pop();
            totalSellSize += limit->getSize();
            totalSellVolume += limit->getTotalVolume();
            if (limit->getLeftChild() != nullptr) {
                queue.push(limit->getLeftChild());
            }
            if (limit->getRightChild() != nullptr) {
                queue.push(limit->getRightChild());
            }
        }
        CHECK(totalSellSize == 4);
        CHECK(totalSellVolume == 40);
    }

    SUBCASE("Get volume at limit price") {
        OrderBook *orderBook = new OrderBook();
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(100, 10, true);

        // Redirect std::cout to a stringstream
        std::stringstream capturedOutput;
        std::streambuf* originalOutputBuffer = std::cout.rdbuf();
        std::cout.rdbuf(capturedOutput.rdbuf());

        orderBook->getVolumeAtLimitPrice(100, true);

        // Restore the original std::cout buffer
        std::cout.rdbuf(originalOutputBuffer);

        // Get the captured output
        std::string output = capturedOutput.str();

        // Check the captured output against the expected output
        CHECK(output == "30\n");
    }

    SUBCASE("Get best bid") {
        OrderBook *orderBook = new OrderBook();
        orderBook->addOrder(100, 10, true);
        orderBook->addOrder(110, 10, true);
        orderBook->addOrder(120, 10, true);

        // Redirect std::cout to a stringstream
        std::stringstream capturedOutput;
        std::streambuf* originalOutputBuffer = std::cout.rdbuf();
        std::cout.rdbuf(capturedOutput.rdbuf());

        orderBook->getBestBid();

        // Restore the original std::cout buffer
        std::cout.rdbuf(originalOutputBuffer);

        // Get the captured output
        std::string output = capturedOutput.str();

        // Check the captured output against the expected output
        CHECK(output == "120\n");
    }
}