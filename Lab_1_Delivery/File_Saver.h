#pragma once
#include <string>
#include "Orders.h"

class FileSaver {
public:
    static void appendCompletedOrder(const Order& order, const std::string& filename);
};