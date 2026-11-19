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
    void clear(); // Для сброса при "выходе из аккаунта"

    void addCourier(Courier c);
    void addProductToCatalog(Product p);

    int createOrder(Client client, int deliveryType);
    bool addItemToOrder(int orderId, int catalogIndex, int quantity);
    void setDeliveryMethod(int orderId, int deliveryType);
    void assignCourier(int orderId, int courierId);
    void advanceOrderStatus(int orderId);

    // Возвращает true, если отмена успешна. Если нужен warning, UI должен проверить needsCancelWarning() заранее
    bool cancelOrder(int orderId);

    void showAllOrders() const;
    void showAllCouriers() const;
    void showCatalog() const;

    const std::vector<Order>& getOrders() const { return activeOrders; }
    const std::vector<Courier>& getCouriers() const { return couriers; }
    const std::vector<Product>& getCatalog() const { return catalog; }
};