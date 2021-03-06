#include "menu.hpp"

extern Invoker inv;

void Menu::addEmployee() {
    std::cout << "to add employee enter department name: ";
    std::string departmentName;
    std::getline(std::cin, departmentName);
    while (!departments->contains(departmentName)) {
        std::cout << "error: wrong department name" << std::endl;
        if (backToMenu()) {
            return;
        }
        std::cout << "to add employee enter department name: ";
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
    size_t id;
    while (!getValidId(id)) {
        if (backToMenu()) {
            return;
        }
        std::cout << "to delete employee enter id: ";
    }
    inv.DeleteEmployee(id);
}

void Menu::deleteDepartment() {
    std::cout << "to delete department enter department name: ";
    std::string departmentName;
    std::getline(std::cin, departmentName);
    while (!departments->contains(departmentName)) {
        std::cout << "error: wrong department name" << std::endl;
        if (backToMenu()) {
            return;
        }
        std::cout << "to delete department enter department name: ";
        std::getline(std::cin, departmentName);
    }
    inv.DeleteDepartment(departmentName);
}

void Menu::editDepartment() {
    std::cout << "You can edit department name." << std::endl << "Enter old department name: ";
    std::string oldDepartmentName;
    std::getline(std::cin, oldDepartmentName);
    while (!departments->contains(oldDepartmentName)) {
        std::cout << "error: wrong department name" << std::endl;
        if (backToMenu()) {
            return;
        }
        std::cout << "to edit department enter old department name: ";
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
    size_t id;
    while (!getValidId(id)) {
        if (backToMenu()) {
            return;
        }
        std::cout << "to edit employee enter id: ";
    }
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
            editEmployeeSurname(id);
            break;
        }
        case 2: {
            editEmployeeName(id);
            break;
        }
        case 3: {
            editEmployeeMiddleName(id);
            break;
        }
        case 4: {
            editEmployeeFunction(id);
            break;
        }
        case 5: {
            editEmployeeSalary(id);
            break;
        }
    }
}

void Menu::editEmployeeSurname(size_t id) {
    std::cout << "Enter new surname: ";
    std::string newSurname;
    while (!getValidName(newSurname)) {
        if (backToMenu()) {
            return;
        }
        std::cout << "Enter new surname: ";
    }
    inv.EditEmployeeSurname(id, newSurname);
}

void Menu::editEmployeeName(size_t id) {
    std::cout << "Enter new name: ";
    std::string newName;
    while (!getValidName(newName)) {
        if (backToMenu()) {
            return;
        }
        std::cout << "Enter new name: ";
    }
    inv.EditEmployeeName(id, newName);
}

void Menu::editEmployeeMiddleName(size_t id) {
    std::cout << "Enter new middle name: ";
    std::string newMiddleName;
    while (!getValidName(newMiddleName)) {
        if (backToMenu()) {
            return;
        }
        std::cout << "Enter new middle name: ";
    }
    inv.EditEmployeeMiddleName(id, newMiddleName);
}

void Menu::editEmployeeFunction(size_t id) {
    std::cout << "Enter new function: ";
    std::string newFunction;
    std::getline(std::cin, newFunction);
    inv.EditEmployeeFunction(id, newFunction);
}

void Menu::editEmployeeSalary(size_t id) {
    std::cout << "Enter new salary as a integer number: ";
    size_t salary = getIntNumber();
    inv.EditEmployeeSalary(id, salary);
}

bool Menu::backToMenu() {
    char exit;
    do {
        std::cout << "You want back to menu? [y/n]: ";
        std::cin >> exit;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (exit == 'Y' || exit == 'y') {
            return true;
        }
    } while (exit != 'N' && exit != 'n');
    return false;
}

std::string Menu::getValidFilePath() {
    std::cout << "enter the file path: ";
    std::string filePath;
    std::getline(std::cin, filePath);
    while (!std::filesystem::exists(filePath)) {
        std::cout << "error! invalid file path" << std::endl;
        if (backToMenu()) {
            return {};
        }
        std::cout << "enter the file path: ";
        std::getline(std::cin, filePath);
    }
    return filePath;
}

