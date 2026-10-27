#pragma once
#include <string>
#include <iostream>

class Delivery {
private:
    int type;
public:
    Delivery(int t = 1) : type(t) {}

    std::string getName() const {
        if (type == 1) return "Стандартная доставка";
        if (type == 2) return "Экспресс-доставка";
        if (type == 3) return "Самовывоз";
        return "Неизвестно";
    }

    double calculateCost(double baseCost) const {
        if (type == 1) return baseCost + 200.0; // +200
        if (type == 2) return baseCost + 500.0; // +500
        if (type == 3) return baseCost;         // Бесплатно
        return baseCost;
    }

    std::string getEstimatedTime() const {
        if (type == 1) return "2 - 4 часа";
        if (type == 2) return " < 1 часа";
        if (type == 3) return "-";
        return "Неизвестно";
    }

    bool requiresCourier() const {
        if (type == 3) return false;
        return true;
    }
};