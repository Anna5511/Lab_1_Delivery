#pragma once
#include <string>
#include <iostream>
#include <iomanip>

class Product {
private:
    std::string name;
    double price;
    int amount;

public:
    Product(std::string n = "", double p = 0.0, int q = 1) : name(n), price(p), amount(q) {}

    std::string getName() const { return name; }
    int getQuantity() const { return amount; }
    double getPrice() const { return price; }

    void changeQuantity(int delta) { amount += delta; }

    void print() const {
        std::cout << std::left
            << std::setw(30) << ("Продукт: " + name)
            << std::setw(15) << (" | Цена: " + std::to_string((int)price) + " р.")
            << " | На полке: " << amount << " шт.\n";
    }
};

class Client {
private:
    int id;
    std::string name;
    std::string phone;

public:
    Client(int i = 0, std::string n = "Гость", std::string p = "0000000000") : id(i), name(n), phone(p) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }

    void print() const {
        std::cout << "Клиент №" << id << ": " << name << " | Телефон: " << phone << "\n";
    }
};

class Courier {
private:
    int id;
    std::string name;
    bool busy;
    std::string phone;

public:
    Courier(int i = 0, std::string n = "Курьер", bool b = false, std::string p = "0000000000")
        : id(i), name(n), busy(b), phone(p) {
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    bool getBusy() const { return busy; }

    void setBusy(bool b) { busy = b; }

    void print() const {
        std::cout << "Курьер №" << id << " : " << std::left << std::setw(15) << name
            << " | Свободен: " << (busy ? "-" : "+") << "\n";
    }
};