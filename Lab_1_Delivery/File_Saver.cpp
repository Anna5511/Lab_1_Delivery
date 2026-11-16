#include "File_Saver.h"
#include <fstream>
#include <iostream>

void FileSaver::saveActiveOrders(const std::vector<Order>& orders, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) { std::cout << "Ошибка: не удалось открыть файл " << filename << "\n"; return; }

    out << "Активные заказы: " << orders.size() << "\n";
    for (size_t i = 0; i < orders.size(); i++) {
        const Order& o = orders[i];
        out << "Заказ №" << o.getId()
            << " | Клиент: " << o.getClient().getName()
            << " | Доставка: " << o.getDelivery().getName()
            << " | Стоимость: " << o.getTotalCost()
            << " | Курьер: " << (o.getCourierId() == -1 ? std::string("нет") : std::to_string(o.getCourierId()))
            << " | Статус: " << statusToString(o.getStatus())
            << " | Товары:";
        for (const Product& p : o.getItems())
            out << " " << p.getName() << "(" << p.getQuantity() << "шт)";
        out << "\n";
    }
}

void FileSaver::appendCompletedOrder(const Order& order, const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (!out) { std::cout << "Ошибка: не удалось открыть файл " << filename << "\n"; return; }

    out << "Заказ №" << order.getId()
        << " | Клиент: " << order.getClient().getName()
        << " | Доставка: " << order.getDelivery().getName()
        << " | Стоимость: " << order.getTotalCost()
        << " | Товары:";
    for (const Product& p : order.getItems())
        out << " " << p.getName() << "(" << p.getQuantity() << "шт)";
    out << " | Статус: Доставлен\n";
}