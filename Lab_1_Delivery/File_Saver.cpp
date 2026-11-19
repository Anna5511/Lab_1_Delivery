#include "File_Saver.h"
#include <fstream>
#include <iostream>

void FileSaver::appendCompletedOrder(const Order& order, const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << "\n";
        return;
    }

    out << "Заказ №" << order.getId()
        << " | Клиент: " << order.getClient().getName()
        << " | Доставка: " << order.getDelivery().getName()
        << " | Стоимость: " << (int)order.getTotalCost() << " р."
        << " | Статус: Завершен\n";
}