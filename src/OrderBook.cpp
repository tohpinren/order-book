//
// Created by Pin Ren Toh on 23/9/23.
//

#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"

#include <unordered_map>
#include <iostream>

/**
 * Constructor for OrderBook.
 */
OrderBook::OrderBook() {
    this->buyTree = nullptr;
    this->sellTree = nullptr;
    this->lowestSell = nullptr;
    this->highestBuy = nullptr;
    this->buyOrders = new std::unordered_map<int, Order *>();
    this->sellOrders = new std::unordered_map<int, Order *>();
    this->buyLimits = new std::unordered_map<float, Limit *>();
    this->sellLimits = new std::unordered_map<float, Limit *>();
    this->currBuyOrdersId = 0;
    this->currSellOrdersId = 0;
    this->profit = 0;
}

/**
 * Getter for buy limit tree.
 *
 * @return Buy limit tree
 */
Limit *OrderBook::getBuyTree() {
    return this->buyTree;
}

/**
 * Getter for sell limit tree.
 *
 * @return Sell limit tree
 */
Limit *OrderBook::getSellTree() {
    return this->sellTree;
}

/**
 * Setter for buy limit tree.
 *
 * @param newBuyTree New buy limit tree
 */
void OrderBook::setBuyTree(Limit *newBuyTree) {
    this->buyTree = newBuyTree;
}

/**
 * Setter for sell limit tree.
 *
 * @param newSellTree New sell limit tree
 */
void OrderBook::setSellTree(Limit *newSellTree) {
    this->sellTree = newSellTree;
}

/**
 * Adds an order to the order book. If limit price does not exist, creates new limit. Else, adds order to limit.
 *
 * @param price Price of the order
 * @param quantity Quantity of the order
 * @param isBuy Boolean indicating if the order is a buy order
 */
void OrderBook::addOrder(float price, int quantity, bool isBuy) {
    time_t timeNow = time(0);
    if (isBuy) {
        Order *newOrder = new Order(currBuyOrdersId, price, quantity, isBuy, timeNow);
        buyOrders->insert(std::make_pair(currBuyOrdersId, newOrder));
        currBuyOrdersId++;

        // If limit price not in tree, create new limit in tree
        if (buyLimits->find(price) == buyLimits->end()) {
            // If limit price not in limits, create new limit in tree
            Limit *newLimit = new Limit(price, isBuy, this);
            buyLimits->insert(std::make_pair(price, newLimit));

            newLimit->addOrder(newOrder);

            if (buyTree == nullptr) {
                buyTree = newLimit;
            } else {
                buyTree->insertLimit(newLimit);
            }
        }
        // If limit price in tree, add order to limit
        else {
            Limit *limit = buyLimits->at(price);
            limit->addOrder(newOrder);
        }

        // If order is highest buy, update highest buy
        if (highestBuy == nullptr || price > highestBuy->getPrice()) {
            highestBuy = newOrder;
        }

        // Print order added
        std::cout << "Buy order added: " << newOrder->getId() << " at " << newOrder->getPrice() << std::endl;
    } else {
        Order *newOrder = new Order(currSellOrdersId, price, quantity, isBuy, timeNow);
        sellOrders->insert(std::make_pair(currSellOrdersId, newOrder));
        currSellOrdersId++;

        // If limit price not in tree, create new limit in tree
        if (sellLimits->find(price) == sellLimits->end()) {
            // If limit price not in limits, create new limit in tree
            Limit *newLimit = new Limit(price, isBuy, this);
            sellLimits->insert(std::make_pair(price, newLimit));

            newLimit->addOrder(newOrder);

            if (sellTree == nullptr) {
                sellTree = newLimit;
            } else {
                sellTree->insertLimit(newLimit);
            }
        }
        // If limit price in tree, add order to limit
        else {
            Limit *limit = sellLimits->at(price);
            limit->addOrder(newOrder);
        }

        // If order is lowest sell, update lowest sell
        if (lowestSell == nullptr || price < lowestSell->getPrice()) {
            lowestSell = newOrder;
        }

        // Print order added
        std::cout << "Sell order added: " << newOrder->getId() << " at " << newOrder->getPrice() << std::endl;
    }
}

/**
 * Cancels an order by removing from Limit. If order does not exist, prints error message.
 * If Limit is empty, Limit is not removed because assuming high volume of orders, Limit will be filled again.
 *
 * @param order Order to be cancelled
 */
void OrderBook::cancelOrder(Order *order) {
    if (order->isBuy()) {
        // Check if order exists
        if (buyOrders->find(order->getId()) == buyOrders->end()) {
            std::cout << "Order does not exist." << std::endl;
            return;
        }

        // Remove order from limit
        Limit *limit = buyLimits->at(order->getPrice());
        limit->removeOrder(order);

        // Print order cancelled
        std::cout << "Buy order cancelled: " << order->getId() << " at " << order->getPrice() << std::endl;

        // If order is highest buy, update highest buy
        if (order == highestBuy) {
            highestBuy = limit->getNextInsideOrder();
        }

        // Remove order from orders map
        buyOrders->erase(order->getId());
    } else {
        // Check if order exists
        if (sellOrders->find(order->getId()) == sellOrders->end()) {
            std::cout << "Order does not exist." << std::endl;
            return;
        }

        // Remove order from limit
        Limit *limit = sellLimits->at(order->getPrice());
        limit->removeOrder(order);

        // Print order cancelled
        std::cout << "Sell order cancelled: " << order->getId() << " at " << order->getPrice() << std::endl;

        // If order is lowest sell, update lowest sell
        if (order == lowestSell) {
            lowestSell = limit->getNextInsideOrder();
        }

        // Remove order from orders map
        sellOrders->erase(order->getId());
    }
}

/**
 * Executes an order if highest buy is greater than or equal to lowest sell.
 */
void OrderBook::executeOrder() {
    if (highestBuy == nullptr || lowestSell == nullptr || highestBuy->getPrice() < lowestSell->getPrice()) {
        std::cout << "There are no orders to execute." << std::endl;
        return;
    }

    if (highestBuy->getQuantity() == lowestSell->getQuantity()) {
        // Remove both from limits
        highestBuy->getParentLimit()->removeOrder(highestBuy);
        lowestSell->getParentLimit()->removeOrder(lowestSell);
        this->profit += (highestBuy->getPrice() - lowestSell->getPrice()) * highestBuy->getQuantity();

        // Remove highest buy and lowest sell from orders map
        buyOrders->erase(highestBuy->getId());
        sellOrders->erase(lowestSell->getId());

        // Print orders executed
        std::cout << "Executed buy order at " << highestBuy->getPrice() << " and sell order at " <<
            lowestSell->getPrice() << std::endl;

        // Update highest buy and lowest sell
        highestBuy = highestBuy->getParentLimit()->getNextInsideOrder();
        lowestSell = lowestSell->getParentLimit()->getNextInsideOrder();
    } else {
        Order *lowerQuantity = highestBuy->getQuantity() < lowestSell->getQuantity() ? highestBuy : lowestSell;
        Order *higherQuantity = highestBuy->getQuantity() < lowestSell->getQuantity() ? lowestSell : highestBuy;

        // Remove lower quantity from limit and update higher quantity
        lowerQuantity->getParentLimit()->removeOrder(lowerQuantity);
        higherQuantity->decreaseQuantity(lowerQuantity->getQuantity());

        if (lowerQuantity->isBuy()) {
            // Remove lower quantity from orders map
            buyOrders->erase(lowerQuantity->getId());

            // Print orders executed, noting which is partial
            std::cout << "Executed buy order at " << lowerQuantity->getPrice() << " and partial sell order at" <<
                      higherQuantity->getPrice() << std::endl;

            // Update highest buy
            highestBuy = highestBuy->getParentLimit()->getNextInsideOrder();

            // Update profit
            // Buy > Sell, lower quantity is the buy order
            this->profit += (lowerQuantity->getPrice() - higherQuantity->getPrice()) * lowerQuantity->getQuantity();
        } else {
            // Remove lower quantity from orders map
            sellOrders->erase(lowerQuantity->getId());

            // Print orders executed, noting which is partial
            std::cout << "Executed partial buy order at " << higherQuantity->getPrice() << " and sell order at" <<
                      lowerQuantity->getPrice() << std::endl;

            // Update lowest sell
            lowestSell = lowestSell->getParentLimit()->getNextInsideOrder();

            // Update profit
            // Buy > Sell, higher quantity is the buy order
            this->profit += (higherQuantity->getPrice() - lowerQuantity->getPrice()) * lowerQuantity->getQuantity();
        }
    }
    // Print profit
    std::cout << "Profit: " << this->profit << std::endl;
}

/**
 * Prints the total volume at a limit price. If limit price does not exist, prints error message.
 *
 * @param price Limit price to get volume at
 * @param isBuy Boolean indicating to check buy or sell tree
 */
void OrderBook::getVolumeAtLimitPrice(float price, bool isBuy) {
    if (isBuy) {
        if (this->buyLimits->find(price) == this->buyLimits->end()) {
            std::cout << "There is no buy volume at this limit price." << std::endl;
            return;
        }
        Limit *limit = this->buyLimits->at(price);
        std::cout << limit->getTotalVolume() << std::endl;
    } else {
        if (this->sellLimits->find(price) == this->sellLimits->end()) {
            std::cout << "There is no sell volume at this limit price." << std::endl;
            return;
        }
        Limit *limit = this->sellLimits->at(price);
        std::cout << limit->getTotalVolume() << std::endl;
    }
}

/**
 * Prints the best ask price. If there is no best ask price, prints error message.
 */
void OrderBook::getBestBid() {
    if (this->highestBuy == nullptr) {
        std::cout << "There is no best bid." << std::endl;
    } else {
        std::cout << this->highestBuy->getPrice() << std::endl;
    }
}
