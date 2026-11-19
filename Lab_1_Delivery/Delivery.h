#pragma once
#include <string>
#include <memory>

class Delivery {
public:
    virtual ~Delivery() = default;
    virtual std::string getName() const = 0;
    virtual double calculateCost(double baseCost) const = 0;
    virtual std::string getEstimatedTime() const = 0;
    virtual bool requiresCourier() const = 0;
    virtual std::unique_ptr<Delivery> clone() const = 0;
};

class StandardDelivery : public Delivery {
public:
    std::string getName() const override { return "Стандартная доставка"; }
    double calculateCost(double baseCost) const override { return baseCost + 200.0; }
    std::string getEstimatedTime() const override { return "1 - 4 часа"; }
    bool requiresCourier() const override { return true; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<StandardDelivery>(*this); }
};

class ExpressDelivery : public Delivery {
public:
    std::string getName() const override { return "Экспресс-доставка"; }
    double calculateCost(double baseCost) const override { return baseCost + 500.0; }
    std::string getEstimatedTime() const override { return "менее 1 часа"; }
    bool requiresCourier() const override { return true; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<ExpressDelivery>(*this); }
};

class PickupDelivery : public Delivery {
public:
    std::string getName() const override { return "Самовывоз"; }
    double calculateCost(double baseCost) const override { return baseCost; }
    std::string getEstimatedTime() const override { return "Самовывоз"; } // Как просили
    bool requiresCourier() const override { return false; }
    std::unique_ptr<Delivery> clone() const override { return std::make_unique<PickupDelivery>(*this); }
};

inline std::unique_ptr<Delivery> createDelivery(int type) {
    switch (type) {
    case 1: return std::make_unique<StandardDelivery>();
    case 2: return std::make_unique<ExpressDelivery>();
    case 3: return std::make_unique<PickupDelivery>();
    default: return nullptr;
    }
}