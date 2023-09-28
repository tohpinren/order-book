//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_LIMIT_H
#define ORDER_BOOK_LIMIT_H

class Order;
class OrderBook;

/**
 * Class representing a limit in the order book.
 */
class Limit {
private:
    /**
     * Price of the limit.
     */
    float price;

    /**
     * Number of orders at the limit.
     */
    int size;

    /**
     * Total quantity of orders at the limit.
     */
    int totalVolume;

    /**
     * Pointer to the parent limit of the limit.
     */
    Limit *parent;

    /**
     * Pointer to the left child of the limit.
     */
    Limit *leftChild;

    /**
     * Pointer to the right child of the limit.
     */
    Limit *rightChild;

    /**
     * Pointer to the head order of the limit.
     */
    Order *headOrder;

    /**
     * Pointer to the tail order of the limit.
     */
    Order *tailOrder;

    /**
     * Height of the limit in the AVL tree.
     */
    int height;

    /**
     * Pointer to the order book.
     */
    OrderBook *orderBook;

    /**
     * Boolean indicating if the limit is a buy limit.
     */
    bool isBuy;

public:
    /**
     * Constructor for Limit.
     *
     * @param price Price of the limit
     * @param isBuy Boolean indicating if the limit is a buy limit
     * @param orderBook Pointer to the order book
     */
    Limit(float price, bool isBuy, OrderBook *orderBook);

    /**
     * Getter for price of the limit.
     *
     * @return Price of the limit
     */
    float getPrice() const;

    /**
     * Getter for boolean indicating if the limit is a buy limit.
     *
     * @return Boolean indicating if the limit is a buy limit
     */
    int getSize() const;

    /**
     * Getter for total volume of the limit.
     *
     * @return Total volume of the limit
     */
    int getTotalVolume() const;

    /**
     * Getter for pointer to the parent limit of the limit.
     *
     * @return Pointer to the parent limit of the limit
     */
    Limit *getParent() const;

    /**
     * Getter for pointer to the left child of the limit.
     *
     * @return Pointer to the left child of the limit
     */
    Limit *getLeftChild() const;

    /**
     * Getter for pointer to the right child of the limit.
     *
     * @return Pointer to the right child of the limit
     */
    Limit *getRightChild() const;

    /**
     * Getter for pointer to the head order of the limit.
     *
     * @return Pointer to the head order of the limit
     */
    Order *getHeadOrder() const;

    /**
     * Getter for pointer to the tail order of the limit.
     *
     * @return Pointer to the tail order of the limit
     */
    Order *getTailOrder() const;

    /**
     * Getter for the height of the limit.
     *
     * @return Height of the limit
     */
    int getHeight() const;

    /**
     * Setter for pointer to the parent limit of the limit.
     *
     * @param parent New pointer to the parent limit of the limit
     */
    void setParent(Limit *parent);

    /**
     * Setter for pointer to the left child of the limit.
     *
     * @param leftChild New pointer to the left child of the limit
     */
    void setLeftChild(Limit *leftChild);

    /**
     * Setter for pointer to the right child of the limit.
     *
     * @param rightChild New pointer to the right child of the limit
     */
    void setRightChild(Limit *rightChild);

    /**
     * Setter for pointer to the head order of the limit.
     *
     * @param headOrder New pointer to the head order of the limit
     */
    void setHeadOrder(Order *headOrder);

    /**
     * Setter for pointer to the tail order of the limit.
     *
     * @param tailOrder New pointer to the tail order of the limit
     */
    void setTailOrder(Order *tailOrder);

    /**
     * Add an order to the limit.
     *
     * @param order Order to add to the limit
     */
    void addOrder(Order *order);

    /**
     * Remove an order from the limit.
     *
     * @param order Order to remove from the limit
     */
    void removeOrder(Order *order);

    /**
     * Insert a limit into the AVL tree.
     *
     * @param limit Limit to insert into the AVL tree
     */
    void insertLimit(Limit *limit);

    /**
     * Update the height of the limit.
     */
    void updateHeight();

    /**
     * Rebalance the AVL tree after adding a limit.
     */
    void rebalanceOnAdd();

    /**
     * Right rotate the AVL tree.
     */
    void rightRotate();

    /**
     * Left rotate the AVL tree.
     */
    void leftRotate();

    /**
     * Increase the size of the limit.
     *
     * @param amount Amount to increase the size of the limit by
     */
    void increaseSize(int amount);

    /**
     * Decrease the size of the limit.
     *
     * @param amount Amount to decrease the size of the limit by
     */
    void decreaseSize(int amount);

    /**
     * Increase the volume of the limit.
     *
     * @param volume Volume to increase the total volume of the limit by
     */
    void increaseVolume(int volume);

    /**
     * Decrease the volume of the limit.
     *
     * @param volume Volume to decrease the total volume of the limit by
     */
    void decreaseVolume(int volume);

    /**
     * Setter for the height of the limit.
     *
     * @param height Height to set the limit to
     */
    void setHeight(int height);

    /**
     * Getter for the next inside order in the linked list.
     *
     * @return Next inside order in the linked list
     */
    Order *getNextInsideOrder() const;
};


#endif //ORDER_BOOK_LIMIT_H
