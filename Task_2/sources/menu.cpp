#include "menu.hpp"

extern Invoker inv;

void Menu::addEmployee() {
    std::cout << "to add employee enter department name: ";
    std::string departmentName;
    std::getline(std::cin, departmentName);
    while (!departments->contains(departmentName)) {
        std::cout << "error: wrong department name" << std::endl
                  << "to add employee enter department name: ";
        std::getline(std::cin, departmentName);
    }
    std::cout << "to add employee enter:" << std::endl << "surname: ";
    std::string surname;
    std::getline(std::cin, surname);
    std::cout << "name: ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "middle name: ";
    std::string middleName;
    std::getline(std::cin, middleName);
    std::cout << "function: ";
    std::string function;
    std::getline(std::cin, function);
    std::cout << "salary as a integer number: ";
    size_t salary = getIntNumber();

    for (auto &[depName, department] : *departments) {
        if (depName == departmentName) {
            inv.setDepartment(&department);
            inv.AddEmployee({surname, name, middleName, function, salary});
            break;
        }
    }
}

void Menu::addDepartament() {
    std::cout << "to add department enter department name: ";
    std::string departmentName;
    std::getline(std::cin, departmentName);
    inv.AddDepartment(departmentName);
}

void Menu::deleteEmployee() {
    std::cout << "to delete employee enter id: ";
    size_t id = getIntNumber();
    inv.DeleteEmployee(id);
}

void Menu::deleteDepartment() {
    std::cout << "to delete department enter department name: ";
    std::string departmentName;
    std::getline(std::cin, departmentName);
    while (!departments->contains(departmentName)) {
        std::cout << "error: wrong department name" << std::endl
                  << "to delete department enter department name: ";
        std::getline(std::cin, departmentName);
    }
    inv.DeleteDepartment(departmentName);
}

void Menu::editDepartment() {
    std::cout << "You can edit department name." << std::endl << "Enter old department name: ";
    std::string oldDepartmentName;
    std::getline(std::cin, oldDepartmentName);
    while (!departments->contains(oldDepartmentName)) {
        std::cout << "error: wrong department name" << std::endl
                  << "to edit department enter old department name: ";
        std::getline(std::cin, oldDepartmentName);
    }
    std::cout << "Enter new department name: ";
    std::string newDepartmentName;
    std::getline(std::cin, newDepartmentName);
    inv.setDepartment(&departments->at(oldDepartmentName));
    inv.EditDepartment(newDepartmentName);
}

void Menu::editEmployee() {
    std::cout << "to edit employee enter id: ";
    size_t id = getIntNumber();
    std::cout << "What you want edit?" << std::endl
              << "1. surname" << std::endl
              << "2. name" << std::endl
              << "3. middle name" << std::endl
              << "4. function" << std::endl
              << "5. salary" << std::endl
              << "selected action: " << std::endl
              << "$ ";
    std::size_t numAction = getVariant(1, 5);
    switch (numAction) {
        case 1: {
            std::cout << "Enter new surname: ";
            std::string newSurname;
            std::getline(std::cin, newSurname);
            for (auto &[departamentName, departament] : *departments) {
                if (departament.setEmployeeSurName(id, newSurname)) {
                    break;
                }
            }
            break;
        }
        case 2: {
            std::cout << "Enter new name: ";
            std::string newName;
            std::getline(std::cin, newName);
            for (auto &[departamentName, departament] : *departments) {
                if (departament.setEmployeeName(id, newName)) {
                    break;
                }
            }
            break;
        }
        case 3: {
            std::cout << "Enter new middle name: ";
            std::string newMiddleName;
            std::getline(std::cin, newMiddleName);
            for (auto &[departamentName, departament] : *departments) {
                if (departament.setEmployeeMiddleName(id, newMiddleName)) {
                    break;
                }
            }
            break;
        }
        case 4: {
            std::cout << "Enter new function: ";
            std::string newFunction;
            std::getline(std::cin, newFunction);
            inv.EditEmployeeFunction(id, newFunction);
            break;
        }
        case 5: {
            std::cout << std::endl << "Enter new salary as a integer number: ";
            size_t salary = getIntNumber();
            inv.EditEmployeeSalary(id, salary);
            break;
        }
    }
}
