#pragma once
#include <vector>
#include "Orders.h"
#include "data.h"

class ServiceManager {
private:
    std::vector<Order> activeOrders;
    std::vector<Courier> couriers;
    std::vector<Product> catalog;
    int nextOrderId;

public:
    ServiceManager();

    void addCourier(Courier c);
    void addProductToCatalog(Product p);

    void createOrder(Client client, int deliveryType);
    void addItemToOrder(int orderId, Product product);
    void assignCourier(int orderId, int courierId);
    void advanceOrderStatus(int orderId);

    void showAllOrders() const;
    void showAllCouriers() const;
    void showCatalog() const;

    std::vector<Order> getOrders() const { return activeOrders; }
};