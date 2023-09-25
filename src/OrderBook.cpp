//
// Created by Pin Ren Toh on 23/9/23.
//

#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"

#include <unordered_map>
#include <iostream>

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
}

Limit *OrderBook::getBuyTree() {
    return this->buyTree;
}

Limit *OrderBook::getSellTree() {
    return this->sellTree;
}

void OrderBook::setBuyTree(Limit *buyTree) {
    this->buyTree = buyTree;
}

void OrderBook::setSellTree(Limit *sellTree) {
    this->sellTree = sellTree;
}

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
    }
}

void OrderBook::cancelOrder(Order *order) {
    if (order->isBuy()) {
        // Check if order exists

        // Remove order from limit

        // If order is highest buy, update highest buy

        // If limit is empty, remove limit from tree

        // If tree is empty, remove tree

        // Remove order from orders map


    } else {
        // Check if order exists

        // Remove order from limit

        // If order is lowest sell, update lowest sell

        // If limit is empty, remove limit from tree

        // If tree is empty, remove tree

        // Remove order from orders map

    }
}

void OrderBook::executeOrder() {
}

void OrderBook::getVolumeAtLimitPrice(float price) {
    if (this->buyLimits->find(price) == this->buyLimits->end()) {
        std::cout << "There is no volume at this limit price." << std::endl;
    } else {
        Limit *limit = this->buyLimits->at(price);
        std::cout << limit->getTotalVolume() << std::endl;
    }
}

void OrderBook::getBestBid() {
    if (this->highestBuy == nullptr) {
        std::cout << "There is no best bid." << std::endl;
    } else {
        std::cout << this->highestBuy->getPrice() << std::endl;
    }
}