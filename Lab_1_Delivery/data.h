#pragma once
#include <string>
#include <iostream>

// товар
class Product {
private:
    std::string name;
    double price;
    int amount;

public:
    Product(std::string n = "", double p = 0.0, int q = 1): name(n), price(p), amount(q) {
    }

    std::string getName() const { return name; }
    int getQuantity() const { return amount; }
    double getPrice() const { return price; }

    void print() const {
        std::cout << "Продукт: " << name << " | Цена: " << price <<" | Количество в магазине: " << amount <<"\n";
    }
};

class Client {
public:
    int id;
    std::string name;
    long long phone;

    Client(int i = 0, std::string n = "Гость", long long p = 0) : id(i), name(n), phone(p) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    long long getPhone() const { return phone; }

    void print() const {
        std::cout << "Клиент №" << id << ": " << name << " | Телефон: " << phone << "\n";
    }
};

class Courier {
public:
    int id;
    std::string name;
    long long phone;
    bool Busy;

    Courier(int i = 0, std::string n = "", bool b = false)
        : id(i), name(n), Busy(b) {
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    long long getPhone() const { return phone; }
    bool getBusy() const { return Busy; }

    void setBusy(bool busy) { Busy = busy; }

    void print() const {
        std::cout << "Курьер №" << id << " : " << name << " | Свободен: " << (Busy ? "-" : "+") << "\n";
    }

};
