//
// Created by Pin Ren Toh on 24/9/23.
//

#include "OrderBook.h"
#include "Order.h"
#include "Limit.h"
#include <iostream>
#include "doctest.h"

int main() {
    doctest::Context context;
    int res = context.run();

    // Test summary
    if (context.shouldExit())
        return res;

    return 0;
}