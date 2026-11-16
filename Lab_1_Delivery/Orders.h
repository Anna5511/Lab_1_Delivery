#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include "data.h"
#include "Delivery.h"

enum class OrderStatus {
    Created,
    In_progress,
    Delivered
};

inline std::string statusToString(OrderStatus s) {
    switch (s) {
    case OrderStatus::Created:     return "Создан";
    case OrderStatus::In_progress: return "В пути";
    case OrderStatus::Delivered:   return "Доставлен";
    }
    return "Неизвестно";
}

class Order {
private:
    int id;
    Client client;
    std::vector<Product> items;
    std::unique_ptr<Delivery> delivery; // полиморфный компонент
    int courierId;
    OrderStatus status;

public:
    Order(int i, Client c, std::unique_ptr<Delivery> d)
        : id(i), client(c), delivery(std::move(d)), courierId(-1), status(OrderStatus::Created) {
    }

    int getId() const { return id; }
    Client getClient() const { return client; }
    const Delivery& getDelivery() const { return *delivery; }
    int getCourierId() const { return courierId; }
    OrderStatus getStatus() const { return status; }
    const std::vector<Product>& getItems() const { return items; }

    // проверка допустимости операции (инкапсуляция)
    bool canAddItems() const { return status == OrderStatus::Created; }

    void addItem(Product p) { items.push_back(p); }

    void setCourier(int cId) { courierId = cId; }

    void setDelivery(std::unique_ptr<Delivery> d) { delivery = std::move(d); }

    void changeStatus() {
        if (status == OrderStatus::Created) status = OrderStatus::In_progress;
        else if (status == OrderStatus::In_progress) status = OrderStatus::Delivered;
    }

    bool canChangeStatus() const {
        // Если статус уже "Доставлен" — менять некуда
        if (status == OrderStatus::Delivered) return false;

        // Если доставка требует курьера — курьер должен быть назначен
        if (delivery->requiresCourier() && courierId == -1) {
            return false;
        }

        return true;
    }

    double getTotalCost() const {
        double sum = 0;
        for (size_t i = 0; i < items.size(); i++)
            sum += items[i].getPrice() * items[i].getQuantity();
        return delivery->calculateCost(sum); // полиморфный вызов, без if-цепочек
    }

    void print() const {
        std::cout << "Заказ №" << id << " | Клиент: " << client.getName() << "\n";
        std::cout << "Доставка: " << delivery->getName()
            << " | Цена: " << getTotalCost() << " руб."
            << " | Срок: " << delivery->getEstimatedTime() << "\n";

        std::cout << "Товары: ";
        for (size_t i = 0; i < items.size(); i++)
            std::cout << items[i].getName() << "(" << items[i].getQuantity() << "шт) ";
        std::cout << "\n";

        std::cout << "Курьер: ";
        if (courierId == -1) std::cout << "Не назначен\n";
        else std::cout << "ID " << courierId << "\n";

        std::cout << "Статус заказа: " << statusToString(status) << "\n";
        std::cout << "------------------------\n";
    }
};