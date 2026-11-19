#include "UI.h"
#include <iostream>
#include <limits>

const int MAX_ORDER_ID = 999999;

UI::UI(ServiceManager& s) : service(s), nextClientId(1), isLoggedIn(false) {}

void UI::login() {
    std::cout << "\n=== ВХОД В СИСТЕМУ ===\n";
    currentUserName = readString("Введите ваше имя: ");
    currentUserPhone = readPhone("Введите ваш телефон (только цифры): ");
    isLoggedIn = true;
    std::cout << "Добро пожаловать, " << currentUserName << "!\n";
}

void UI::printMenu() const {
    std::cout << "\n--------- Меню --------- \n";
    std::cout << "1) Создать заказ\n";
    std::cout << "2) Показать список заказов\n";
    std::cout << "3) Выбрать способ доставки\n";
    std::cout << "4) Назначить курьера\n";
    std::cout << "5) Изменить статус заказа\n";
    std::cout << "6) Отменить заказ\n";
    std::cout << "7) Выйти в меню входа (сбросить данные)\n";
    std::cout << "8) Выход из программы\n";
}

int UI::readInt(const std::string& prompt, int minVal, int maxVal) const {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= minVal && v <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cout << "Некорректный ввод. Нужно целое число от " << minVal << " до " << maxVal << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// НАДЕЖНАЯ ВАЛИДАЦИЯ ТЕЛЕФОНА: устраняет баг с "5vndji"
std::string UI::readPhone(const std::string& prompt) const {
    std::string s;
    while (true) {
        std::cout << prompt;
        std::cin >> s;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool ok = true;
        for (char c : s) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                ok = false;
                break;
            }
        }
        if (ok && !s.empty()) return s;

        std::cout << "Ошибка: телефон должен состоять только из цифр.\n";
    }
}

std::string UI::readString(const std::string& prompt) const {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

char UI::readYesNo(const std::string& prompt) const {
    char c;
    while (true) {
        std::cout << prompt;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 'д' || c == 'Д' || c == 'y' || c == 'Y') return 'y';
        if (c == 'н' || c == 'Н' || c == 'n' || c == 'N') return 'n';
        std::cout << "Введите 'д' или 'н'.\n";
    }
}

void UI::flowCreateOrder() {
    std::cout << "\n--- Создание заказа ---\n";
    std::cout << "Данные клиента: " << currentUserName << ", " << currentUserPhone << "\n";

    // Собираем товары и отслеживаем, сколько уже выбрано каждого
    std::vector<std::pair<int, int>> selectedItems; // {индекс в каталоге, количество}
    std::vector<int> selectedQuantities(service.getCatalog().size(), 0); // сколько уже выбрано каждого товара

    while (true) {
        std::cout << "\nКаталог товаров:\n";
        service.showCatalog();
        int idx = readInt("Номер товара для добавления (0 - завершить): ", 0,
            static_cast<int>(service.getCatalog().size()));
        if (idx == 0) break;

        const Product& p = service.getCatalog()[idx - 1];
        int catalogIndex = idx - 1;

        // Проверяем, есть ли товар на полке
        if (p.getQuantity() <= 0) {
            std::cout << "Ошибка: Товар \"" << p.getName() << "\" закончился!\n";
            continue;
        }

        // Проверяем, сколько ещё можно выбрать с учётом уже выбранного в этой сессии
        int alreadySelected = selectedQuantities[catalogIndex];
        int availableNow = p.getQuantity() - alreadySelected;

        if (availableNow <= 0) {
            std::cout << "Ошибка: Вы уже выбрали весь доступный товар \"" << p.getName() << "\"!\n";
            continue;
        }

        // Запрашиваем количество, разрешаем 0 как отказ
        std::cout << "Количество (доступно ещё " << availableNow << " шт, 0 - отказаться): ";
        int qty;
        while (true) {
            std::cin >> qty;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (qty == 0) {
                std::cout << "Товар \"" << p.getName() << "\" не добавлен.\n";
                break; // Выход из цикла, возвращаемся к выбору товара
            }

            if (qty >= 1 && qty <= availableNow) {
                selectedItems.push_back({ catalogIndex, qty });
                selectedQuantities[catalogIndex] += qty;
                std::cout << "Добавлено: " << p.getName() << " - " << qty << " шт.\n";
                break;
            }

            std::cout << "Некорректный ввод. Введите число от 1 до " << availableNow
                << " или 0 для отказа: ";
        }
    }

    // Если пользователь не выбрал ни одного товара — отменяем создание заказа
    if (selectedItems.empty()) {
        std::cout << "\nЗаказ не создан: вы не добавили ни одного товара.\n";
        return;
    }

    // Теперь спрашиваем способ доставки
    int type = readInt("Способ доставки (1 - стандарт (+200р), 2 - экспресс (+500р), 3 - самовывоз (+0р)): ", 1, 3);

    // Создаём заказ
    Client client(nextClientId++, currentUserName, currentUserPhone);
    int orderId = service.createOrder(client, type);
    if (orderId == 0) return;

    // Добавляем все выбранные товары (ошибок уже не будет, т.к. всё проверили выше)
    for (const auto& item : selectedItems) {
        service.addItemToOrder(orderId, item.first, item.second);
    }

    std::cout << "\nЗаказ сформирован:\n";
    service.showAllOrders();
}

void UI::flowChooseDelivery() {
    std::cout << "\n--- Выбор способа доставки ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, MAX_ORDER_ID);
    int type = readInt("Новый способ (1 - стандарт (+200р), 2 - экспресс (+500р), 3 - самовывоз (+0р)): ", 1, 3);
    service.setDeliveryMethod(orderId, type);
}

void UI::flowAssignCourier() {
    std::cout << "\n--- Назначение курьера ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, MAX_ORDER_ID);
    std::cout << "Список курьеров:\n";
    service.showAllCouriers();
    int courierId = readInt("ID курьера: ", 1, MAX_ORDER_ID);
    service.assignCourier(orderId, courierId);
}

void UI::flowChangeStatus() {
    std::cout << "\n--- Смена статуса ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа: ", 1, MAX_ORDER_ID);
    service.advanceOrderStatus(orderId);
}

void UI::flowCancelOrder() {
    std::cout << "\n--- Отмена заказа ---\n";
    service.showAllOrders();
    int orderId = readInt("ID заказа для отмены: ", 1, MAX_ORDER_ID);

    bool found = false;
    bool needsWarning = false;
    for (const auto& order : service.getOrders()) {
        if (order.getId() == orderId) {
            found = true;
            needsWarning = order.needsCancelWarning();
            break;
        }
    }

    if (!found) {
        std::cout << "Ошибка: Заказ не найден.\n";
        return;
    }

    if (needsWarning) {
        std::cout << "ВНИМАНИЕ: Курьер уже назначен. Стоимость доставки возвращена не будет!\n";
    }

    char confirm = readYesNo("Отменить заказ? (д/н): ");
    if (confirm == 'n') {
        std::cout << "Отмена прервана.\n";
        return;
    }

    service.cancelOrder(orderId);
}

void UI::run() {
    while (true) {
        if (!isLoggedIn) {
            login();
        }

        bool inSession = true;
        while (inSession) {
            printMenu();
            int choice = readInt("Ваш выбор: ", 1, 8);
            switch (choice) {
            case 1: flowCreateOrder(); break;
            case 2: service.showAllOrders(); break;
            case 3: flowChooseDelivery(); break;
            case 4: flowAssignCourier(); break;
            case 5: flowChangeStatus(); break;
            case 6: flowCancelOrder(); break;
            case 7:
                std::cout << "Выход в меню входа. Все текущие заказы будут потеряны.\n";
                service.clear();
                isLoggedIn = false;
                inSession = false;
                break;
            case 8:
                std::cout << "Выход из программы.\n";
                return; // Полный выход из run(), программа завершится
            }
        }
    }
}