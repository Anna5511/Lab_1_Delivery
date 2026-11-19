#include "Service_Manager.h"
#include "File_Saver.h"
#include <iostream>

ServiceManager::ServiceManager() : nextOrderId(1) {}

void ServiceManager::clear() {
    activeOrders.clear();
    nextOrderId = 1;
    // Каталог и курьеры можно перезаполнить в main, или оставить как есть, 
    // но для "чистого" старта лучше сбросить и каталог, если он был динамическим.
    // В текущей архитектуре main.cpp заполняет его заново при новом цикле.
}

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

bool ServiceManager::addItemToOrder(int orderId, int catalogIndex, int quantity) {
    if (catalogIndex < 0 || catalogIndex >= static_cast<int>(catalog.size())) {
        std::cout << "Ошибка: Неверный индекс товара.\n";
        return false;
    }

    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            if (!activeOrders[i].canAddItems()) {
                std::cout << "Ошибка: в этот заказ уже нельзя добавлять позиции.\n";
                return false;
            }

            Product& catalogItem = catalog[catalogIndex];
            if (catalogItem.getQuantity() < quantity) {
                std::cout << "Ошибка: Недостаточно товара \"" << catalogItem.getName()
                    << "\" в магазине! (Доступно: " << catalogItem.getQuantity() << ")\n";
                return false;
            }

            // Уменьшаем количество на полке (без пересоздания объекта!)
            catalogItem.changeQuantity(-quantity);

            // Добавляем в заказ
            activeOrders[i].addItem(Product(catalogItem.getName(), catalogItem.getPrice(), quantity));

            std::cout << "Товар \"" << catalogItem.getName() << "\" (" << quantity
                << " шт) добавлен. Осталось на полке: " << catalogItem.getQuantity() << " шт.\n";
            return true;
        }
    }
    std::cout << "Ошибка: Заказ с таким ID не найден.\n";
    return false;
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
            std::cout << "Способ доставки обновлен.\n";
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
            activeOrders[i].changeStatus();

            if (activeOrders[i].getStatus() == OrderStatus::Delivered ||
                activeOrders[i].getStatus() == OrderStatus::Completed) {
                std::cout << "Заказ №" << orderId << " завершен!\n";

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

bool ServiceManager::cancelOrder(int orderId) {
    for (size_t i = 0; i < activeOrders.size(); i++) {
        if (activeOrders[i].getId() == orderId) {
            // Возвращаем товары в каталог
            const auto& items = activeOrders[i].getItems();
            for (const auto& item : items) {
                for (auto& catItem : catalog) {
                    if (catItem.getName() == item.getName()) {
                        catItem.changeQuantity(item.getQuantity());
                        break;
                    }
                }
            }

            // Освобождаем курьера, если он был назначен
            int cid = activeOrders[i].getCourierId();
            if (cid != -1) {
                for (size_t j = 0; j < couriers.size(); j++) {
                    if (couriers[j].getId() == cid) {
                        couriers[j].setBusy(false);
                        break;
                    }
                }
            }

            activeOrders.erase(activeOrders.begin() + i);
            std::cout << "Заказ №" << orderId << " успешно отменен. Товары возвращены на полки.\n";
            return true;
        }
    }
    std::cout << "Ошибка: Заказ не найден.\n";
    return false;
}

void ServiceManager::showAllOrders() const {
    if (activeOrders.empty()) {
        std::cout << "Активных заказов нет.\n";
        return;
    }
    for (size_t i = 0; i < activeOrders.size(); i++) {
        activeOrders[i].print();
    }
}

void ServiceManager::showAllCouriers() const {
    for (size_t i = 0; i < couriers.size(); i++) couriers[i].print();
}

void ServiceManager::showCatalog() const {
    for (size_t i = 0; i < catalog.size(); i++) {
        std::cout << i + 1 << ". ";
        catalog[i].print();
    }
}