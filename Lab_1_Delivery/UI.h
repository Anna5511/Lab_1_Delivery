#pragma once
#include <string>
#include "Service_Manager.h"

class UI {
private:
    ServiceManager& service;
    int nextClientId;
    bool isLoggedIn;

    std::string currentUserName;
    std::string currentUserPhone;

    void printMenu() const;
    int  readInt(const std::string& prompt, int minVal, int maxVal) const;
    std::string readPhone(const std::string& prompt) const;
    std::string readString(const std::string& prompt) const;
    char readYesNo(const std::string& prompt) const;   // <-- ÄÎÁÀÂÈÒÜ ÝÒÓ ÑÒÐÎÊÓ

    void login();
    void flowCreateOrder();
    void flowChooseDelivery();
    void flowAssignCourier();
    void flowChangeStatus();
    void flowCancelOrder();

public:
    UI(ServiceManager& s);
    void run();
};