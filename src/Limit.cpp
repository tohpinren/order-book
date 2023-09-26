//
// Created by Pin Ren Toh on 23/9/23.
//

#include "Limit.h"
#include "OrderBook.h"
#include "Order.h"

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

int Limit::getSize() const {
    return size;
}

int Limit::getTotalVolume() const {
    return totalVolume;
}

Limit *Limit::getParent() const {
    return parent;
}

Limit *Limit::getLeftChild() const {
    return leftChild;
}

Limit *Limit::getRightChild() const {
    return rightChild;
}

Order *Limit::getHeadOrder() const {
    return headOrder;
}

Order *Limit::getTailOrder() const {
    return tailOrder;
}

int Limit::getHeight() const {
    return height;
}

void Limit::increaseSize(int quantity) {
    this->size += quantity;
}

void Limit::decreaseSize(int quantity) {
    this->size -= quantity;
}

void Limit::increaseVolume(int volume) {
    this->totalVolume += volume;
}

void Limit::decreaseVolume(int volume) {
    this->totalVolume -= volume;
}

void Limit::setParent(Limit *parent) {
    this->parent = parent;
}

void Limit::setLeftChild(Limit *leftChild) {
    this->leftChild = leftChild;
}

void Limit::setRightChild(Limit *rightChild) {
    this->rightChild = rightChild;
}

void Limit::setSize(int size) {

}

void Limit::setHeadOrder(Order *headOrder) {
    this->headOrder = headOrder;
}

void Limit::setTailOrder(Order *tailOrder) {
    this->tailOrder = tailOrder;
}

void Limit::addOrder(Order *order) {
    if (headOrder == nullptr) {
        headOrder = order;
        tailOrder = order;
    } else {
        tailOrder->nextOrder = order;
        order->prevOrder = tailOrder;
        this->tailOrder = order;
    }
    this->size += 1;
    this->totalVolume += order->quantity;
    order->parentLimit = this;
}

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

void Limit::setHeight(int height) {
    this->height = height;
}

void Limit::updateHeight() {
    int prevHeight = this->getHeight();
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
        Limit *leftChild = curr->getLeftChild();
        int leftLeftHeight;
        int leftRightHeight;
        if (leftChild->getLeftChild() == nullptr) {
            leftLeftHeight = -1;
        } else {
            leftLeftHeight = leftChild->getLeftChild()->getHeight();
        }
        if (leftChild->getRightChild() == nullptr) {
            leftRightHeight = -1;
        } else {
            leftRightHeight = leftChild->getRightChild()->getHeight();
        }
        if (leftLeftHeight >= leftRightHeight) {
            curr->rightRotate();
        }
        // If left child is right heavy, left rotate left child then right rotate
        else {
            leftChild->leftRotate();
            curr->rightRotate();
        }
    }
    // If right heavy, check right child
    else {
        Limit *rightChild = curr->getRightChild();
        int rightLeftHeight;
        int rightRightHeight;
        if (rightChild->getLeftChild() == nullptr) {
            rightLeftHeight = -1;
        } else {
            rightLeftHeight = rightChild->getLeftChild()->getHeight();
        }
        if (rightChild->getRightChild() == nullptr) {
            rightRightHeight = -1;
        } else {
            rightRightHeight = rightChild->getRightChild()->getHeight();
        }
        // If right child is equi-height or right heavy, left rotate
        if (rightRightHeight >= rightLeftHeight) {
            curr->leftRotate();
        }
        // If right child is left heavy, right rotate right child then left rotate
        else {
            rightChild->rightRotate();
            curr->leftRotate();
        }
    }
}

void Limit::leftRotate() {
    Limit *rightChild = this->getRightChild();
    rightChild->parent = this->getParent();
    this->parent = rightChild;
    this->rightChild = rightChild->getLeftChild();
    rightChild->leftChild = this;

    // Update height
    this->setHeight(std::max(this->getLeftChild()->getHeight(), this->getRightChild()->getHeight()) + 1);
    rightChild->setHeight(std::max(rightChild->getLeftChild()->getHeight(), rightChild->getRightChild()->getHeight()) + 1);

    // Update root
    if (this->orderBook->getBuyTree() == this) {
        this->orderBook->setBuyTree(rightChild);
    } else if (this->orderBook->getSellTree() == this) {
        this->orderBook->setSellTree(rightChild);
    }
}

void Limit::rightRotate() {
    Limit *leftChild = this->getLeftChild();
    leftChild->parent = this->getParent();
    this->parent = leftChild;
    this->leftChild = leftChild->getRightChild();
    leftChild->rightChild = this;

    // Update height
    this->setHeight(std::max(this->getLeftChild()->getHeight(), this->getRightChild()->getHeight()) + 1);
    leftChild->setHeight(std::max(leftChild->getLeftChild()->getHeight(), leftChild->getRightChild()->getHeight()) + 1);

    // Update root
    if (this->orderBook->getBuyTree() == this) {
        this->orderBook->setBuyTree(leftChild);
    } else if (this->orderBook->getSellTree() == this) {
        this->orderBook->setSellTree(leftChild);
    }
}

float Limit::getPrice() const {
    return price;
}

void Limit::removeOrder(Order *order) {
    if (this->headOrder == order && this->tailOrder == order) {
        this->headOrder = nullptr;
        this->tailOrder = nullptr;
    } else if (this->headOrder == order) {
        this->headOrder = order->nextOrder;
        order->nextOrder->prevOrder = nullptr;
    } else if (this->tailOrder == order) {
        this->tailOrder = order->prevOrder;
        order->prevOrder->nextOrder = nullptr;
    } else {
        order->prevOrder->nextOrder = order->nextOrder;
        order->nextOrder->prevOrder = order->prevOrder;
    }
    this->size -= 1;
    this->totalVolume -= order->quantity;
}

Order *Limit::getNextInsideOrder(bool isBuy) {
    if (this->headOrder != nullptr) {
        return this->headOrder;
    }
    if (isBuy) {
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