#pragma once
#include <string>
#include <memory>

// јбстрактный контракт способа доставки
class Delivery {
public:
    virtual ~Delivery() = default;

    virtual std::string getName() const = 0;
    virtual double calculateCost(double baseCost) const = 0;
    virtual std::string getEstimatedTime() const = 0;
    virtual bool requiresCourier() const = 0;

    // нужен, чтобы заказ можно было скопировать/сохранить при желании
    virtual std::unique_ptr<Delivery> clone() const = 0;
};

class StandardDelivery : public Delivery {
public:
    std::string getName() const override { return "—тандартна€ доставка"; }
    double calculateCost(double baseCost) const override { return baseCost + 200.0; }
    std::string getEstimatedTime() const override { return "2 - 4 часа"; }
    bool requiresCourier() const override { return true; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<StandardDelivery>(*this); }
};

class ExpressDelivery : public Delivery {
public:
    std::string getName() const override { return "Ёкспресс-доставка"; }
    double calculateCost(double baseCost) const override { return baseCost + 500.0; }
    std::string getEstimatedTime() const override { return "менее 1 часа"; }
    bool requiresCourier() const override { return true; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<ExpressDelivery>(*this); }
};

class PickupDelivery : public Delivery {
public:
    std::string getName() const override { return "—амовывоз"; }
    double calculateCost(double baseCost) const override { return baseCost; }
    std::string getEstimatedTime() const override { return "готово через 30 минут"; }
    bool requiresCourier() const override { return false; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<PickupDelivery>(*this); }
};

// ‘абрика: превращает число из консоли в объект нужного класса
inline std::unique_ptr<Delivery> createDelivery(int type) {
    switch (type) {
    case 1: return std::make_unique<StandardDelivery>();
    case 2: return std::make_unique<ExpressDelivery>();
    case 3: return std::make_unique<PickupDelivery>();
    default: return nullptr; // некорректный ввод Ч обработает вызывающий код
    }
}