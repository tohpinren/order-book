//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_ORDERBOOK_H
#define ORDER_BOOK_ORDERBOOK_H

#include <unordered_map>
#include "Order.h"
#include "Limit.h"

class OrderBook {
public:
    OrderBook();

    void addOrder(float price, int quantity, bool isBuy);

    void cancelOrder(Order *order);

    void executeOrder();

    void getVolumeAtLimitPrice(float price);

    void getBestBid();

    Limit *getBuyTree();

    Limit *getSellTree();

    void setBuyTree(Limit *buyTree);

    void setSellTree(Limit *sellTree);

    Limit *buyTree;

    Limit *sellTree;

    Order *lowestSell;

    Order *highestBuy;

    std::unordered_map<int, Order *> *buyOrders;

    std::unordered_map<int, Order *> *sellOrders;

    std::unordered_map<float, Limit *> *buyLimits;

    std::unordered_map<float, Limit *> *sellLimits;

    int currBuyOrdersId;

    int currSellOrdersId;

    float profit;
};


#endif //ORDER_BOOK_ORDERBOOK_H
