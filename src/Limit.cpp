//
// Created by Pin Ren Toh on 23/9/23.
//

#include "Limit.h"
#include "OrderBook.h"
#include "Order.h"

/**
 * Constructor for Limit.
 *
 * @param price Price of the limit
 * @param isBuy Boolean indicating if the limit is a buy limit
 * @param orderBook Pointer to the order book
 */
Limit::Limit(float price, bool isBuy, OrderBook *orderBook) {
    this->price = price;
    this->size = 0;
    this->totalVolume = 0;
    this->parent = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->headOrder = nullptr;
    this->tailOrder = nullptr;
    this->height = 0;
    this->orderBook = orderBook;
    this->isBuy = isBuy;
}

/**
 * Getter for size of the limit.
 *
 * @return Size of the limit
 */
int Limit::getSize() const {
    return size;
}

/**
 * Getter for total volume of the limit.
 *
 * @return Total volume of the limit
 */
int Limit::getTotalVolume() const {
    return totalVolume;
}

/**
 * Getter for pointer to the parent limit of the limit.
 *
 * @return Pointer to the parent limit of the limit
 */
Limit *Limit::getParent() const {
    return parent;
}

/**
 * Getter for pointer to the left child of the limit.
 *
 * @return Pointer to the left child of the limit
 */
Limit *Limit::getLeftChild() const {
    return leftChild;
}

/**
 * Getter for pointer to the right child of the limit.
 *
 * @return Pointer to the right child of the limit
 */
Limit *Limit::getRightChild() const {
    return rightChild;
}

/**
 * Getter for pointer to the head order of the limit.
 *
 * @return Pointer to the head order of the limit
 */
Order *Limit::getHeadOrder() const {
    return headOrder;
}

/**
 * Getter for pointer to the tail order of the limit.
 *
 * @return Pointer to the tail order of the limit
 */
Order *Limit::getTailOrder() const {
    return tailOrder;
}

/**
 * Getter for the height of the limit.
 *
 * @return Height of the limit
 */
int Limit::getHeight() const {
    return height;
}

/**
 * Getter for the price of the limit.
 *
 * @return Price of the limit
 */
float Limit::getPrice() const {
    return price;
}

/**
 * Increase the size of the limit by the given amount.
 *
 * @param amount Amount to increase the size by
 */
void Limit::increaseSize(int amount) {
    this->size += amount;
}

/**
 * Decrease the size of the limit by the given amount.
 *
 * @param amount Amount to decrease the size by
 */
void Limit::decreaseSize(int amount) {
    this->size -= amount;
}

/**
 * Increase the total volume of the limit by the given volume.
 *
 * @param volume Volume to increase the total volume by
 */
void Limit::increaseVolume(int volume) {
    this->totalVolume += volume;
}

/**
 * Decrease the total volume of the limit by the given volume.
 *
 * @param volume Volume to decrease the total volume by
 */
void Limit::decreaseVolume(int volume) {
    this->totalVolume -= volume;
}

/**
 * Setter for the parent limit of the limit.
 *
 * @param newParent New parent limit of the limit
 */
void Limit::setParent(Limit *newParent) {
    this->parent = newParent;
}

/**
 * Setter for the left child of the limit.
 *
 * @param newLeftChild New left child of the limit
 */
void Limit::setLeftChild(Limit *newLeftChild) {
    this->leftChild = newLeftChild;
}

/**
 * Setter for the right child of the limit.
 *
 * @param newRightChild New right child of the limit
 */
void Limit::setRightChild(Limit *newRightChild) {
    this->rightChild = newRightChild;
}

/**
 * Add an order to the limit.
 *
 * @param order Order to add to the limit
 */
void Limit::addOrder(Order *order) {
    if (this->headOrder == nullptr) {
        this->headOrder = order;
        this->tailOrder = order;
    } else {
        this->tailOrder->setNextOrder(order);
        order->setPrevOrder(tailOrder);
        this->tailOrder = order;
    }
    this->increaseSize(1);
    this->increaseVolume(order->getQuantity());
    order->setParentLimit(this);
}

/**
 * Insert a limit into the limit AVL tree.
 *
 * @param limit Limit to insert into the limit AVL tree
 */
void Limit::insertLimit(Limit *limit) {
    if (limit->getPrice() < this->getPrice()) {
        if (this->getLeftChild() == nullptr) {
            this->setLeftChild(limit);
            limit->setParent(this);
            limit->updateHeight();
            limit->rebalanceOnAdd();
        } else {
            this->getLeftChild()->insertLimit(limit);
        }
    } else {
        if (this->getRightChild() == nullptr) {
            this->setRightChild(limit);
            limit->setParent(this);
        } else {
            this->getRightChild()->insertLimit(limit);
        }
    }
}

/**
 * Setter for the height of the limit.
 *
 * @param newHeight New height of the limit
 */
void Limit::setHeight(int newHeight) {
    this->height = newHeight;
}

/**
 * Update the height of the limit.
 */
void Limit::updateHeight() {
    Limit *curr = this->getParent();

    while (curr != nullptr) {
        int leftHeight;
        if (curr->getLeftChild() == nullptr) {
            leftHeight = -1;
        } else {
            leftHeight = curr->getLeftChild()->getHeight();
        }
        int rightHeight;
        if (curr->getRightChild() == nullptr) {
            rightHeight = -1;
        } else {
            rightHeight = curr->getRightChild()->getHeight();
        }
        curr->setHeight(std::max(leftHeight, rightHeight) + 1);
        curr = curr->getParent();
    }
}

/**
 * Rebalance the limit AVL tree on add.
 */
void Limit::rebalanceOnAdd() {
    // Find the first height unbalanced node
    Limit *curr = this->getParent();
    bool isLeftHeavy;
    while (curr != nullptr) {
        int leftHeight;
        if (curr->getLeftChild() == nullptr) {
            leftHeight = -1;
        } else {
            leftHeight = curr->getLeftChild()->getHeight();
        }
        int rightHeight;
        if (curr->getRightChild() == nullptr) {
            rightHeight = -1;
        } else {
            rightHeight = curr->getRightChild()->getHeight();
        }

        // Check if height unbalanced and left heavy or right heavy
        if (leftHeight - rightHeight > 1) {
            isLeftHeavy = true;
            break;
        } else if (rightHeight - leftHeight > 1) {
            isLeftHeavy = false;
            break;
        }

        curr = curr->getParent();
    }

    if (curr == nullptr) {
        return;
    }

    // If left heavy, check left child
    if (isLeftHeavy) {
        // If left child is equi-height or left heavy, right rotate
        Limit *left = curr->getLeftChild();
        int leftLeftHeight;
        int leftRightHeight;
        if (left->getLeftChild() == nullptr) {
            leftLeftHeight = -1;
        } else {
            leftLeftHeight = left->getLeftChild()->getHeight();
        }
        if (left->getRightChild() == nullptr) {
            leftRightHeight = -1;
        } else {
            leftRightHeight = left->getRightChild()->getHeight();
        }
        if (leftLeftHeight >= leftRightHeight) {
            curr->rightRotate();
        }
        // If left child is right heavy, left rotate left child then right rotate
        else {
            left->leftRotate();
            curr->rightRotate();
        }
    }
    // If right heavy, check right child
    else {
        Limit *right = curr->getRightChild();
        int rightLeftHeight;
        int rightRightHeight;
        if (right->getLeftChild() == nullptr) {
            rightLeftHeight = -1;
        } else {
            rightLeftHeight = right->getLeftChild()->getHeight();
        }
        if (right->getRightChild() == nullptr) {
            rightRightHeight = -1;
        } else {
            rightRightHeight = right->getRightChild()->getHeight();
        }
        // If right child is equi-height or right heavy, left rotate
        if (rightRightHeight >= rightLeftHeight) {
            curr->leftRotate();
        }
        // If right child is left heavy, right rotate right child then left rotate
        else {
            right->rightRotate();
            curr->leftRotate();
        }
    }
}

/**
 * Left rotate the limit.
 */
void Limit::leftRotate() {
    Limit *right = this->getRightChild();
    right->setParent(this->getParent());
    this->parent = right;
    this->rightChild = right->getLeftChild();
    right->setLeftChild(this);

    // Update height
    this->setHeight(std::max(this->getLeftChild()->getHeight(),
                             this->getRightChild()->getHeight()) + 1);
    right->setHeight(std::max(right->getLeftChild()->getHeight(),
                                   right->getRightChild()->getHeight()) + 1);

    // Update root
    if (this->orderBook->getBuyTree() == this) {
        this->orderBook->setBuyTree(right);
    } else if (this->orderBook->getSellTree() == this) {
        this->orderBook->setSellTree(right);
    }
}

/**
 * Right rotate the limit.
 */
void Limit::rightRotate() {
    Limit *left = this->getLeftChild();
    left->setParent(this->getParent());
    this->parent = left;
    this->leftChild = left->getRightChild();
    left->rightChild = this;

    // Update height
    this->setHeight(std::max(this->getLeftChild()->getHeight(),
                             this->getRightChild()->getHeight()) + 1);
    left->setHeight(std::max(left->getLeftChild()->getHeight(),
                                  left->getRightChild()->getHeight()) + 1);

    // Update root
    if (this->orderBook->getBuyTree() == this) {
        this->orderBook->setBuyTree(left);
    } else if (this->orderBook->getSellTree() == this) {
        this->orderBook->setSellTree(left);
    }
}

/**
 * Remove an order from the limit.
 *
 * @param order Order to remove from the limit
 */
void Limit::removeOrder(Order *order) {
    if (this->headOrder == order && this->tailOrder == order) {
        this->setHeadOrder(nullptr);
        this->setTailOrder(nullptr);
    } else if (this->headOrder == order) {
        this->setHeadOrder(order->getNextOrder());
        order->getNextOrder()->setPrevOrder(nullptr);
    } else if (this->tailOrder == order) {
        this->setTailOrder(order->getPrevOrder());
        order->getPrevOrder()->setNextOrder(nullptr);
    } else {
        order->getPrevOrder()->setNextOrder(order->getNextOrder());
        order->getNextOrder()->setPrevOrder(order->getPrevOrder());
    }
    this->decreaseSize(1);
    this->increaseVolume(order->getQuantity());
}

/**
 * Getter for the next inside order in the order book.
 *
 * @return Next inside order in the order book
 */
Order *Limit::getNextInsideOrder() const {
    if (this->headOrder != nullptr) {
        return this->headOrder;
    }
    if (this->isBuy) {
        /*
         * Find next highest buy limit. If limit contained highest order, it must be the bottom right of the tree.
         * It will not have any right children. Since tree is balanced, limit will have at most a height 0 left child.
         * Next highest buy limit will be either the parent or it's left child.
         */
        if (this->leftChild != nullptr) {
            return this->leftChild->getHeadOrder();
        } else if (this->parent != nullptr) {
            return this->parent->getHeadOrder();
        } else {
            return nullptr;
        }
    } else {
        /*
         * Find next lowest sell limit. If limit contained lowest order, it must be the bottom left of the tree.
         * It will not have any left children. Since tree is balanced, limit will have at most a height 0 right child.
         * Next lowest sell limit will be either the parent or it's right child.
         */
        if (this->rightChild != nullptr) {
            return this->rightChild->getHeadOrder();
        } else if (this->parent != nullptr) {
            return this->parent->getHeadOrder();
        } else {
            return nullptr;
        }
    }
}

/**
 * Setter for pointer to the head order of the limit.
 *
 * @param newHeadOrder New pointer to the head order of the limit
 */
void Limit::setHeadOrder(Order *newHeadOrder) {
    this->headOrder = newHeadOrder;
}

/**
 * Setter for pointer to the tail order of the limit.
 *
 * @param newTailOrder New pointer to the tail order of the limit
 */
void Limit::setTailOrder(Order *newTailOrder) {
    this->tailOrder = newTailOrder;
}
