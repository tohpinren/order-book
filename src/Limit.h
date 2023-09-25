//
// Created by Pin Ren Toh on 23/9/23.
//

#ifndef ORDER_BOOK_LIMIT_H
#define ORDER_BOOK_LIMIT_H

class Order;
class OrderBook;

class Limit {
public:

    Limit(float price, bool isBuy, OrderBook *orderBook);

    float getPrice() const;

    int getSize() const;

    int getTotalVolume() const;

    Limit *getParent() const;

    Limit *getLeftChild() const;

    Limit *getRightChild() const;

    Order *getHeadOrder() const;

    Order *getTailOrder() const;

    void setSize(int size);

    void setTotalVolume(int totalVolume);

    void setParent(Limit *parent);

    void setLeftChild(Limit *leftChild);

    void setRightChild(Limit *rightChild);

    void setHeadOrder(Order *headOrder);

    void setTailOrder(Order *tailOrder);

    void addOrder(Order *order);

    void removeOrder(Order *order);

    void insertLimit(Limit *limit);

    void updateHeight();

    void rebalanceOnAdd();

    void rightRotate();

    void leftRotate();

    float price;

    int size;

    int totalVolume;

    Limit *parent;

    Limit *leftChild;

    Limit *rightChild;

    Order *headOrder;

    Order *tailOrder;

    int height;

    OrderBook *orderBook;

    bool isBuy;

    int getHeight() const;

    void increaseSize(int quantity);

    void decreaseSize(int quantity);

    void increaseVolume(int volume);

    void decreaseVolume(int volume);

    void setHeight(int height);

    Order *getNextInsideOrder(bool isBuy);
};


#endif //ORDER_BOOK_LIMIT_H
