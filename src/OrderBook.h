//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_ORDERBOOK_H
#define ORDER_BOOK_ORDERBOOK_H

#include <unordered_map>
#include "Order.h"
#include "Limit.h"

/**
 * Class representing the order book.
 */
class OrderBook {
private:
    /**
     * Pointer to the root of the buy limit AVL tree.
     */
    Limit *buyTree;

    /**
     * Pointer to the root of the sell limit AVL tree.
     */
    Limit *sellTree;

    /**
     * Pointer to the lowest sell order.
     */
    Order *lowestSell;

    /**
     * Pointer to the highest buy order.
     */
    Order *highestBuy;

    /**
     * Map of buy orders.
     */
    std::unordered_map<int, Order *> *buyOrders;

    /**
     * Map of sell orders.
     */
    std::unordered_map<int, Order *> *sellOrders;

    /**
     * Map of buy limits.
     */
    std::unordered_map<float, Limit *> *buyLimits;

    /**
     * Map of sell limits.
     */
    std::unordered_map<float, Limit *> *sellLimits;

    /**
     * ID of the next buy order.
     */
    int currBuyOrdersId;

    /**
     * ID of the next sell order.
     */
    int currSellOrdersId;

    /**
     * Current total profits of the order book.
     */
    float profit;
public:
    /**
     * Constructor for OrderBook.
     */
    OrderBook();

    /**
     * Add order to the order book.
     *
     * @param price Price of the order
     * @param quantity Quantity of the order
     * @param isBuy Boolean indicating if the order is a buy order
     */
    Order *addOrder(float price, int quantity, bool isBuy);

    /**
     * Cancel order in the order book.
     *
     * @param order Order to cancel
     */
    void cancelOrder(Order *order);

    /**
     * Execute order in the order book.
     */
    void executeOrder();

    /**
     * Getter for the volume at a given price.
     *
     * @param price Price to get volume at
     * @param isBuy Boolean indicating to check buy or sell side
     */
    void getVolumeAtLimitPrice(float price, bool isBuy);

    /**
     * Getter for the best bid.
     */
    void getBestBid();

    /**
     * Getter for limit buy tree.
     *
     * @return Limit buy tree
     */
    Limit *getBuyTree();

    /**
     * Getter for limit sell tree.
     *
     * @return Limit sell tree
     */
    Limit *getSellTree();

    /**
     * Setter for limit buy tree.
     *
     * @param buyTree New limit buy tree
     */
    void setBuyTree(Limit *buyTree);

    /**
     * Setter for limit sell tree.
     *
     * @param sellTree New limit sell tree
     */
    void setSellTree(Limit *sellTree);
};


#endif //ORDER_BOOK_ORDERBOOK_H
