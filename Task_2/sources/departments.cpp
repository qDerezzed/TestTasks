#include <departments.hpp>

static size_t idNum = 0;

Department::Department(std::string inputName, size_t inputEmployeesAmount, double inputAverageSalary,
                       std::map<size_t, Employee> inputEmployees) : departmentName(std::move(inputName)),
                                                                    employeesAmount(inputEmployeesAmount),
                                                                    averageSalary(inputAverageSalary),
                                                                    employees(std::move(inputEmployees)) {
    for (const auto &employee : employees) {
        curTotalSalary += employee.second.salary;
    }
    idNum = employees.size();
}

size_t Department::addEmployee(const Employee &employee) {
    employees.insert({idNum++, employee});
    curTotalSalary += employee.salary;
    ++employeesAmount;
    averageSalary = curTotalSalary / employeesAmount;
    return idNum - 1;
}

size_t Department::addEmployee(const std::pair<size_t, Employee> &employee) {
    employees.insert(employee);
    curTotalSalary += employee.second.salary;
    ++employeesAmount;
    averageSalary = curTotalSalary / employeesAmount;
    return employee.first;
}

bool Department::deleteEmployee(size_t id, std::pair<size_t, Employee> &employee) {
    if (!employees.contains(id) || employeesAmount == 0) {
        return false;
    }
    employee = {id, employees[id]};
    --employeesAmount;
    if (employeesAmount != 0) {
        averageSalary = curTotalSalary / employeesAmount;
    } else {
        averageSalary = 0;
    }
    employees.erase(id);
    return true;
}

bool Department::deleteEmployee(size_t id) {
    if (!employees.contains(id) || employeesAmount == 0) {
        return false;
    }
    curTotalSalary -= employees[id].salary;
    --employeesAmount;
    if (employeesAmount != 0) {
        averageSalary = curTotalSalary / employeesAmount;
    } else {
        averageSalary = 0;
    }
    employees.erase(id);
    return true;
}

std::string Department::setEmployeeName(size_t id, const std::string &inputName) {
    if (!employees.contains(id)) {
        return {};
    }
    std::string oldName = std::move(employees[id].name);
    employees[id].name = inputName;
    return oldName;
}

std::string Department::setEmployeeMiddleName(size_t id, const std::string &inputName) {
    if (!employees.contains(id)) {
        return {};
    }
    std::string oldMiddleName = std::move(employees[id].middleName);
    employees[id].middleName = inputName;
    return oldMiddleName;
}

std::string Department::setEmployeeSurName(size_t id, const std::string &inputName) {
    if (!employees.contains(id)) {
        return {};
    }
    std::string oldSurname = std::move(employees[id].surname);
    employees[id].surname = inputName;
    return oldSurname;
}

std::string Department::setEmployeeFunction(size_t id, const std::string &inputFunction) {
    if (!employees.contains(id)) {
        return {};
    }
    std::string oldFunction = std::move(employees[id].function);
    employees[id].function = inputFunction;
    return oldFunction;
}

bool Department::setEmployeeSalary(size_t id, int inputSalary, int &oldSalary) {
    if (!employees.contains(id) || employeesAmount == 0) {
        return false;
    }
    curTotalSalary -= employees[id].salary;
    curTotalSalary += inputSalary;
    oldSalary = employees[id].salary;
    employees[id].salary = inputSalary;
    if (employeesAmount != 0) {
        averageSalary = curTotalSalary / employeesAmount;
    }
    return true;
}

void Department::printDepartament() const {
    for (const auto &[id, employee] : employees) {
        std::cout << "  +-id: " << id << " surname: " << employee.surname
                  << " name: " << employee.name << " middle_name: " << employee.middleName
                  << " function: " << employee.function << " salary: " << employee.salary << std::endl;
    }
    std::cout << std::endl;
}
