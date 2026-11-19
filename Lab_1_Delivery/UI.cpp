#include "UI.h"
#include "File_Saver.h"
#include <iostream>
#include <limits>

UI::UI(ServiceManager& s) : service(s), nextClientId(1) {}

void UI::printMenu() const {
    std::cout << "\n--------- Меню --------- \n";
    std::cout << "1) Создать заказ и добавить позиции\n";
    std::cout << "2) Показать список заказов\n";
    std::cout << "3) Выбрать способ доставки для заказа\n";
    std::cout << "4) Назначить курьера\n";
    std::cout << "5) Изменить статус заказа\n";
    std::cout << "6) Выйти\n";
}

int UI::readInt(const std::string& prompt, int minVal, int maxVal) const {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= minVal && v <= maxVal)
            return v;
        std::cout << "Некорректный ввод. Нужно целое число от " << minVal << " до " << maxVal << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

long long UI::readPhone(const std::string& prompt) const {
    long long v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= 0) return v;
        std::cout << "Некорректный номер телефона.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string UI::readString(const std::string& prompt) const {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void UI::flowCreateOrder() {
    std::cout << "\n--- Создание заказа ---\n";

    // Сначала собираем данные клиента
    std::string name = readString("Имя клиента: ");
    long long phone = readPhone("Телефон клиента: ");
    int type = readInt("Способ доставки (1 - стандарт, 2 - экспресс, 3 - самовывоз): ", 1, 3);

    // Теперь собираем товары во временный список
    std::vector<Product> selectedItems;

    while (true) {
        std::cout << "\nКаталог товаров:\n";
        service.showCatalog();
        int idx = readInt("Номер товара для добавления (0 - завершить): ", 0,
            static_cast<int>(service.getCatalog().size()));
        if (idx == 0) break;

        const Product& p = service.getCatalog()[idx - 1];
        int qty = readInt("Количество: ", 1, p.getQuantity());
        selectedItems.push_back(Product(p.getName(), p.getPrice(), qty));
    }

    // Если пользователь не выбрал ни одного товара — отменяем создание заказа
    if (selectedItems.empty()) {
        std::cout << "\nЗаказ не создан: вы не добавили ни одного товара.\n";
        return;
    }

    // Только теперь создаём заказ и добавляем в него товары
    Client client(nextClientId++, name, phone);
    int orderId = service.createOrder(client, type);
    if (orderId == 0) return;

    for (size_t i = 0; i < selectedItems.size(); i++) {
        service.addItemToOrder(orderId, selectedItems[i]);
    }

    std::cout << "\nЗаказ сформирован. Текущая информация:\n";
    service.showAllOrders();
}

void UI::flowChooseDelivery() {
    std::cout << "\n--- Выбор способа доставки ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, 1000000);
    int type = readInt("Новый способ (1 - стандарт, 2 - экспресс, 3 - самовывоз): ", 1, 3);
    service.setDeliveryMethod(orderId, type);
}

void UI::flowAssignCourier() {
    std::cout << "\n--- Назначение курьера ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, 1000000);
    std::cout << "Список курьеров:\n";
    service.showAllCouriers();
    int courierId = readInt("ID курьера: ", 1, 1000000);
    service.assignCourier(orderId, courierId);
}

void UI::flowChangeStatus() {
    std::cout << "\n--- Смена статуса ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, 1000000);
    service.advanceOrderStatus(orderId);
}

void UI::run() {
    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Ваш выбор: ", 1, 6);
        switch (choice) {
        case 1: flowCreateOrder();     break;
        case 2: service.showAllOrders(); break;
        case 3: flowChooseDelivery();  break;
        case 4: flowAssignCourier();   break;
        case 5: flowChangeStatus();    break;
        case 6:
            FileSaver::saveActiveOrders(service.getOrders(), "active_orders.txt");
            std::cout << "Работа завершена. Активные заказы сохранены в active_orders.txt\n";
            running = false;
            break;
        }
    }
}