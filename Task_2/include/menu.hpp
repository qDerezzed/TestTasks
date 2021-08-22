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

    static bool getIntNumber(size_t &number) {
        std::string str;
        getline(std::cin, str);
        try {
            number = std::stoi(str);
        }
        catch (...) {
            std::cout << "error! need to enter a integer number" << std::endl;
            return false;
        }
        return true;
    }

    static size_t getIntNumber() {
        std::string str;
        size_t number = 0;
        bool flag = false;
        do {
            getline(std::cin, str);
            try {
                number = std::stoi(str);
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
