#include "Service_Manager.h"
#include "File_Saver.h"
#include <iostream>

ServiceManager::ServiceManager() : nextOrderId(1) {}

void ServiceManager::addCourier(Courier c) { couriers.push_back(c); }
void ServiceManager::addProductToCatalog(Product p) { catalog.push_back(p); }

int ServiceManager::createOrder(Client client, int deliveryType) {
    auto d = createDelivery(deliveryType);
    if (!d) {
        std::cout << "Ошибка: неверный тип доставки.\n";
        return 0;
    }
    activeOrders.push_back(Order(nextOrderId, client, std::move(d)));
    std::cout << "Успех! Создан заказ №" << nextOrderId << "\n";
    return nextOrderId++;
}

void ServiceManager::addItemToOrder(int orderId, Product product) {
    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            if (!activeOrders[i].canAddItems()) {
                std::cout << "Ошибка: в этот заказ уже нельзя добавлять позиции.\n";
                return;
            }

            // Проверяем, хватает ли товара в каталоге
            bool enoughStock = false;
            for (size_t k = 0; k < catalog.size(); k++) {
                if (catalog[k].getName() == product.getName()) {
                    if (catalog[k].getQuantity() >= product.getQuantity()) {
                        enoughStock = true;
                    }
                    break;
                }
            }

            if (!enoughStock) {
                std::cout << "Ошибка: Недостаточно товара \"" << product.getName()
                    << "\" в магазине!\n";
                return;
            }

            // Добавляем товар в заказ
            activeOrders[i].addItem(product);

            // Уменьшаем количество в каталоге
            for (size_t k = 0; k < catalog.size(); k++) {
                if (catalog[k].getName() == product.getName()) {
                    // Создаём новый Product с уменьшенным количеством
                    Product updated(catalog[k].getName(),
                        catalog[k].getPrice(),
                        catalog[k].getQuantity() - product.getQuantity());
                    catalog[k] = updated;
                    std::cout << "Товар \"" << product.getName()
                        << "\" (" << product.getQuantity() << " шт) добавлен в заказ №"
                        << orderId << ". Осталось в магазине: "
                        << catalog[k].getQuantity() << " шт.\n";
                    return;
                }
            }

            return;
        }
    }
    std::cout << "Ошибка: Заказ с таким ID не найден.\n";
}

void ServiceManager::setDeliveryMethod(int orderId, int deliveryType) {
    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            if (activeOrders[i].getStatus() != OrderStatus::Created) {
                std::cout << "Ошибка: способ доставки можно менять только у заказа в статусе «Создан».\n";
                return;
            }
            auto d = createDelivery(deliveryType);
            if (!d) { std::cout << "Ошибка: неверный тип доставки.\n"; return; }

            activeOrders[i].setDelivery(std::move(d));
            std::cout << "Способ доставки для заказа №" << orderId << " установлен.\n";
            std::cout << "Итоговая стоимость: " << activeOrders[i].getTotalCost()
                << " руб. | Срок: " << activeOrders[i].getDelivery().getEstimatedTime() << "\n";
            return;
        }
    }
    std::cout << "Ошибка: Заказ не найден.\n";
}

void ServiceManager::assignCourier(int orderId, int courierId) {
    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            if (activeOrders[i].getStatus() != OrderStatus::Created) {
                std::cout << "Ошибка: курьера можно назначать только заказу в статусе «Создан».\n";
                return;
            }
            if (!activeOrders[i].getDelivery().requiresCourier()) {
                std::cout << "Ошибка: Для самовывоза курьер не требуется!\n";
                return;
            }
            for (size_t j = 0; j < couriers.size(); j++) {
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
    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            // Проверяем, можно ли менять статус
            if (!activeOrders[i].canChangeStatus()) {
                if (activeOrders[i].getDelivery().requiresCourier() &&
                    activeOrders[i].getCourierId() == -1) {
                    std::cout << "Ошибка: Нельзя изменить статус — курьер не назначен!\n";
                }
                else {
                    std::cout << "Ошибка: Заказ уже доставлен, статус нельзя изменить.\n";
                }
                return;
            }

            activeOrders[i].changeStatus();

            if (activeOrders[i].getStatus() == OrderStatus::Delivered) {
                std::cout << "Заказ №" << orderId << " доставлен!\n";

                int cid = activeOrders[i].getCourierId();
                if (cid != -1) {
                    for (size_t j = 0; j < couriers.size(); j++)
                        if (couriers[j].getId() == cid) couriers[j].setBusy(false);
                }

                FileSaver::appendCompletedOrder(activeOrders[i], "completed_orders.txt");
                activeOrders.erase(activeOrders.begin() + i);
            }
            else {
                std::cout << "Статус заказа №" << orderId << " обновлен: "
                    << statusToString(activeOrders[i].getStatus()) << ".\n";
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
    for (size_t i = 0; i < activeOrders.size(); i++)
        activeOrders[i].print();
}

void ServiceManager::showAllCouriers() const {
    for (size_t i = 0; i < couriers.size(); i++)
        couriers[i].print();
}

void ServiceManager::showCatalog() const {
    for (size_t i = 0; i < catalog.size(); i++) {
        std::cout << i + 1 << ". ";
        catalog[i].print();
    }
}