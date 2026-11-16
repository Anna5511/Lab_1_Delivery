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

    int  createOrder(Client client, int deliveryType);
    void addItemToOrder(int orderId, Product product);
    void setDeliveryMethod(int orderId, int deliveryType);
    void assignCourier(int orderId, int courierId);
    void advanceOrderStatus(int orderId);

    void showAllOrders() const;
    void showAllCouriers() const;
    void showCatalog() const;

    const std::vector<Order>& getOrders()   const { return activeOrders; }
    const std::vector<Courier>& getCouriers() const { return couriers; }
    const std::vector<Product>& getCatalog()  const { return catalog; }
};