#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "data.h"
#include "Delivery.h"

enum class OrderStatus {
    Created,
    In_progress,
    Delivered
};

class Order {
private:
    int id;
    Client client;
    std::vector<Product> items;
    Delivery delivery;
    int courierId;
    OrderStatus status;

public:
    Order(int i, Client c, Delivery d) : id(i), client(c), delivery(d), courierId(-1), status(OrderStatus::Created) {}

    int getId() const { return id; }
    Client getClient() const { return client; }
    Delivery getDelivery() const { return delivery; }
    int getCourierId() const { return courierId; }
    OrderStatus getStatus() const { return status; }

    void addItem(Product p) {
        items.push_back(p);
    }

    void setCourier(int cId) {
        courierId = cId;
    }

    void changeStatus() {
        if (status == OrderStatus::Created) status = OrderStatus::In_progress;
        else if (status == OrderStatus::In_progress) status = OrderStatus::Delivered;
    }

    double getTotalCost() {
        double sum = 0;
        for (int i = 0; i < items.size(); i++) {
            sum += items[i].getPrice() * items[i].getQuantity();
        }
        return delivery.calculateCost(sum);
    }

    void print() {
        std::cout << "Заказ №" << id << " | Клиент: " << client.getName() << "\n";
        std::cout << "Доставка: " << delivery.getName() << " | Цена: " << getTotalCost() << " руб.\n";

        std::cout << "Товары: ";
        for (int i = 0; i < items.size(); i++) {
            std::cout << items[i].getName() << "(" << items[i].getQuantity() << "шт) ";
        }
        std::cout << "\n";

        std::cout << "Курьер: ";
        if (courierId == -1) std::cout << "Не назначен\n";
        else std::cout << "ID " << courierId << "\n";

        std::cout << "Статус заказа: ";
        if (status == OrderStatus::Created) std::cout << "Создан\n";
        else if (status == OrderStatus::In_progress) std::cout << "В пути\n";
        else std::cout << "Доставлен\n";
        std::cout << "------------------------\n";
    }
};