#pragma once
#pragma once
#include <string>
#include "Orders.h"

class FileSaver {
public:
    static void saveActiveOrders(const std::vector<Order>& orders, const std::string& filename);
    static void appendCompletedOrder(const Order& order, const std::string& filename);
};