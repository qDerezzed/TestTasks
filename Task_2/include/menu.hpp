#ifndef AVSOFT_TASK_MENU_HPP
#define AVSOFT_TASK_MENU_HPP

#include <iostream>

#include "departments.hpp"
#include <invoker.hpp>

class Menu {
public:
    Menu() = delete;

    Menu &operator=(const Menu &) = delete;

    ~Menu() = default;

    explicit Menu(std::map<std::string, Department> *inputDepartments) : departments(inputDepartments) {}

    static void printMenu() {
        std::cout << "menu" << std::endl
                  << "1. print tree of departments" << std::endl
                  << "2. add employee" << std::endl
                  << "3. add department" << std::endl
                  << "4. edit employee" << std::endl
                  << "5. edit department" << std::endl
                  << "6. delete employee" << std::endl
                  << "7. delete department" << std::endl
                  << "8. undo action" << std::endl
                  << "9. exit" << std::endl
                  << "selected action: " << std::endl
                  << "$ ";
    }

    static size_t getVariant(size_t leftBoard, size_t rightBoard) {
        std::string str;
        size_t variant = 0;
        do {
            getline(std::cin, str);
            try {
                variant = std::stoul(str);
                if (variant < leftBoard || variant > rightBoard) {
                    std::cout << "error! need to enter a number from " << leftBoard << " to " << rightBoard << ": ";
                    continue;
                }
            }
            catch (...) {
                std::cout << "error! need to enter a number from " << leftBoard << " to " << rightBoard << ": ";
            }
        } while (variant < leftBoard || variant > rightBoard);
        return variant;
    }

    bool getValidId(size_t &id) const {
        std::string str;
        getline(std::cin, str);
        if (str[0] == '-') {
            std::cout << "error! need to enter a positive integer number: ";
            return false;
        }
        try {
            id = std::stoull(str);
        }
        catch (...) {
            std::cout << "error! need to enter a integer number" << std::endl;
            return false;
        }
        if (!containsId(id)) {
            std::cout << "error: wrong id" << std::endl;
            return false;
        }
        return true;
    }

    static bool getValidName(std::string &name) {
        getline(std::cin, name);
        for (char i : name) {
            if (!isalpha(i)) {
                std::cout << "error! a string can contain only letters" << std::endl;
                return false;
            }
        }
        return true;
    }

    static size_t getIntNumber() {
        std::string str;
        size_t number = 0;
        bool flag = false;
        do {
            getline(std::cin, str);
            if (str[0] == '-') {
                std::cout << "error! need to enter a positive integer number: ";
                continue;
            }
            try {
                number = std::stoull(str);
                flag = true;
            }
            catch (...) {
                std::cout << "error! need to enter a integer number: ";
            }
        } while (!flag);
        return number;
    }

    void addEmployee();

    void addDepartament();

    void deleteEmployee();

    void deleteDepartment();

    void editDepartment();

    void editEmployee();

    bool containsId(size_t id) const {
        for (const auto &department : *departments) {
            if (department.second.getEmployees().contains(id)) {
                return true;
            }
        }
        return false;
    }

private:
    void editEmployeeSurname(size_t id);

    void editEmployeeName(size_t id);

    void editEmployeeMiddleName(size_t id);

    void editEmployeeFunction(size_t id);

    void editEmployeeSalary(size_t id);

    static bool backToMenu();

    std::map<std::string, Department> *departments = nullptr;
};


#endif //AVSOFT_TASK_MENU_HPP
