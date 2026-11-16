#pragma once
#include <string>
#include "Service_Manager.h"

class UI {
private:
    ServiceManager& service;
    int nextClientId;

    void printMenu() const;
    int  readInt(const std::string& prompt, int minVal, int maxVal) const;
    long long readPhone(const std::string& prompt) const;
    std::string readString(const std::string& prompt) const;

    void flowCreateOrder();
    void flowChooseDelivery();
    void flowAssignCourier();
    void flowChangeStatus();

public:
    UI(ServiceManager& s);
    void run();
};