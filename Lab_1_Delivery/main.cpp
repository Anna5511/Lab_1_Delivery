#include "Service_Manager.h"
#include "UI.h"
#include <clocale>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);
    std::setlocale(LC_ALL, "Russian");

    ServiceManager service;

    // Инициализация каталога
    service.addProductToCatalog(Product("Хлеб 300г", 100, 15));
    service.addProductToCatalog(Product("Молоко 930 мл", 92, 15));
    service.addProductToCatalog(Product("Яйца куриные 10 шт", 150, 50));
    service.addProductToCatalog(Product("Шоколад Milka 80г", 130, 20));

    // Инициализация курьеров
    service.addCourier(Courier(1, "Саша", false, "89201112233"));
    service.addCourier(Courier(2, "Таня", false, "89211112233"));
    service.addCourier(Courier(3, "Виталий", false, "89221112233"));
    service.addCourier(Courier(4, "Василий", false, "89231112233"));
    service.addCourier(Courier(5, "Тайлер", false, "89231112233"));

    UI ui(service);
    ui.run();

    return 0;
}