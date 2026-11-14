#include "Service_Manager.h"
#include "File_Saver.h"
#include <iostream>

ServiceManager::ServiceManager() : nextOrderId(1) {}

void ServiceManager::addCourier(Courier c) { couriers.push_back(c); }
void ServiceManager::addProductToCatalog(Product p) { catalog.push_back(p); }

void ServiceManager::createOrder(Client client, int deliveryType) {
    Delivery d(deliveryType);
    Order newOrder(nextOrderId, client, d);
    activeOrders.push_back(newOrder);
    std::cout << "Успех! Создан заказ №" << nextOrderId << "\n";
    nextOrderId++;
}

void ServiceManager::addItemToOrder(int orderId, Product product) {
    for (int i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            activeOrders[i].addItem(product);
            std::cout << "Товар добавлен в заказ №" << orderId << "\n";
            return;
        }
    }
    std::cout << "Ошибка: Заказ с таким ID не найден.\n";
}

void ServiceManager::assignCourier(int orderId, int courierId) {
    for (int i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            if (!activeOrders[i].getDelivery().requiresCourier()) {
                std::cout << "Ошибка: Для самовывоза курьер не требуется!\n";
                return;
            }
            for (int j = 0; j < couriers.size(); j++) {
                if (couriers[j].getId() == courierId) {
                    if (couriers[j].getBusy()) {
                        std::cout << "Ошибка: Курьер уже занят.\n";
                        return;
                    }
                    activeOrders[i].setCourier(courierId);
                    couriers[j].setBusy(true);
                    std::cout << "Курьер №" << courierId << " назначен на заказ №" << orderId << "\n";
                    return;
                }
            }
            std::cout << "Ошибка: Курьер с таким ID не найден.\n";
            return;
        }
    }
    std::cout << "Ошибка: Заказ не найден.\n";
}

void ServiceManager::advanceOrderStatus(int orderId) {
    for (int i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            activeOrders[i].changeStatus();

            if (activeOrders[i].getStatus() == OrderStatus::Delivered) {
                std::cout << "Заказ №" << orderId << " доставлен!\n";

                // Освобождаем курьера
                int cid = activeOrders[i].getCourierId();
                if (cid != -1) {
                    for (int j = 0; j < couriers.size(); j++) {
                        if (couriers[j].getId() == cid) couriers[j].setBusy(false);
                    }
                }
                // ТУТ ПОЗЖЕ БУДЕТ СОХРАНЕНИЕ В ФАЙЛ
                // FileSaver::appendCompletedOrder(activeOrders[i], "completed_orders.txt");

                activeOrders.erase(activeOrders.begin() + i);
            }
            else {
                std::cout << "Статус заказа №" << orderId << " обновлен.\n";
            }
            return;
        }
    }
    std::cout << "Ошибка: Заказ не найден.\n";
}

void ServiceManager::showAllOrders() const {
    if (activeOrders.empty()) {
        std::cout << "Активных заказов нет.\n";
        return;
    }
    for (int i = 0; i < activeOrders.size(); i++) {
        activeOrders[i].print();
    }
}

void ServiceManager::showAllCouriers() const {
    for (int i = 0; i < couriers.size(); i++) couriers[i].print();
}

void ServiceManager::showCatalog() const {
    for (int i = 0; i < catalog.size(); i++) {
        std::cout << i + 1 << ". ";
        catalog[i].print();
    }
}