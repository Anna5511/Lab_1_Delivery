#include "Service_Manager.h"
#include "UI.h"

int main() {
    ServiceManager service;

    // Демо-каталог
    service.addProductToCatalog(Product("Пицца Маргарита", 450, 20));
    service.addProductToCatalog(Product("Бургер", 350, 15));
    service.addProductToCatalog(Product("Кола 0.5", 100, 50));
    service.addProductToCatalog(Product("Картошка фри", 150, 30));
    service.addProductToCatalog(Product("Чизкейк", 250, 10));

    // Демо-курьеры
    service.addCourier(Courier(1, "Иван", false, 89001112233));
    service.addCourier(Courier(2, "Мария", false, 89002223344));
    service.addCourier(Courier(3, "Алексей", false, 89003334455));

    UI ui(service);
    ui.run();

    return 0;
}