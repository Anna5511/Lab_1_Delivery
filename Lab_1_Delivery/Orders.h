#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include "data.h"
#include "Delivery.h"

enum class OrderStatus {
    Created,
    Assembling,   // Для самовывоза
    Ready,        // Для самовывоза
    Completed,    // Для самовывоза (забран)
    In_progress,  // Для курьера
    Delivered     // Для курьера
};

inline std::string statusToString(OrderStatus s) {
    switch (s) {
    case OrderStatus::Created:     return "Создан";
    case OrderStatus::Assembling:  return "В сборке";
    case OrderStatus::Ready:       return "Готов к выдаче";
    case OrderStatus::Completed:   return "Заказ завершен (забран)";
    case OrderStatus::In_progress: return "В пути (у курьера)";
    case OrderStatus::Delivered:   return "Доставлен";
    }
    return "Неизвестно";
}

class Order {
private:
    int id;
    Client client;
    std::vector<Product> items;
    std::unique_ptr<Delivery> delivery;
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

    bool canAddItems() const { return status == OrderStatus::Created; }

    // Логика отмены: самовывоз можно отменить до Completed, курьерскую - только Created (или с предупреждением)
    bool canCancelSoft() const {
        if (delivery->getName() == "Самовывоз") {
            return status != OrderStatus::Completed;
        }
        return status == OrderStatus::Created && courierId == -1;
    }

    bool needsCancelWarning() const {
        return delivery->getName() != "Самовывоз" && courierId != -1;
    }

    void addItem(Product p) { items.push_back(p); }
    void setCourier(int cId) { courierId = cId; }
    void setDelivery(std::unique_ptr<Delivery> d) { delivery = std::move(d); }

    void changeStatus() {
        if (delivery->getName() == "Самовывоз") {
            if (status == OrderStatus::Created) status = OrderStatus::Assembling;
            else if (status == OrderStatus::Assembling) status = OrderStatus::Ready;
            else if (status == OrderStatus::Ready) status = OrderStatus::Completed;
        }
        else {
            if (status == OrderStatus::Created) status = OrderStatus::In_progress;
            else if (status == OrderStatus::In_progress) status = OrderStatus::Delivered;
        }
    }

    double getTotalCost() const {
        double sum = 0;
        for (size_t i = 0; i < items.size(); i++)
            sum += items[i].getPrice() * items[i].getQuantity();
        return delivery->calculateCost(sum);
    }

    void print() const {
        std::cout << "Заказ №" << id << "\n";
        std::cout << "Имя клиента: " << client.getName() << "\n";
        std::cout << "Номер: " << client.getPhone() << "\n";
        std::cout << "----\nЗаказ:\n";

        double itemsSum = 0;
        for (size_t i = 0; i < items.size(); i++) {
            double lineSum = items[i].getPrice() * items[i].getQuantity();
            itemsSum += lineSum;
            std::cout << "  " << (i + 1) << ". "
                << std::left << std::setw(25) << items[i].getName()
                << std::setw(4) << items[i].getQuantity() << " шт. x "
                << std::setw(4) << (int)items[i].getPrice() << " р. = "
                << (int)lineSum << " р.\n";
        }

        std::cout << "---\n";
        std::cout << "Доставка: " << delivery->getName() << "\n";
        std::cout << "Время доставки: " << delivery->getEstimatedTime() << "\n";
        double deliveryCost = getTotalCost() - itemsSum;
        std::cout << "Стоимость доставки: " << (int)deliveryCost << " р.\n";
        std::cout << "----\n";
        std::cout << "Итог: " << (int)getTotalCost() << " р.\n";
        std::cout << "Статус: " << statusToString(status) << "\n";
        std::cout << "========================\n";
    }
};