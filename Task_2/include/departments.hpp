#ifndef AVSOFT_TASK_DEPARTMENTS_HPP
#define AVSOFT_TASK_DEPARTMENTS_HPP

#include <iostream>
#include <map>
#include <utility>

struct Employee {
    std::string surname;
    std::string name;
    std::string middleName;
    std::string function;
    size_t salary = 0;
};

class Department {
public:

    Department() = default;

    Department(std::string inputName, size_t inputEmployeesAmount, double inputAverageSalary,
               std::map<size_t, Employee> inputEmployees);

    explicit Department(std::string inputName) : departmentName(std::move(inputName)) {}

    ~Department() = default;

    size_t addEmployee(const Employee &employee);

    size_t addEmployee(const std::pair<size_t, Employee> &employee);

    bool deleteEmployee(size_t id, std::pair<size_t, Employee> &employee);

    bool deleteEmployee(size_t id);

    void setDepartmentName(const std::string &inputName) {
        departmentName = inputName;
    }

    std::string setEmployeeName(size_t id, const std::string &inputName);

    std::string setEmployeeMiddleName(size_t id, const std::string &inputName);

    std::string setEmployeeSurName(size_t id, const std::string &inputName);

    std::string setEmployeeFunction(size_t id, const std::string &inputFunction);

    bool setEmployeeSalary(size_t id, int inputSalary, int &oldSalary);

    [[nodiscard]] std::string getDepartmentName() const {
        return departmentName;
    }

    [[nodiscard]] size_t getEmployeesAmount() const {
        return employeesAmount;
    }

    [[nodiscard]] double getAverageSalary() const {
        return averageSalary;
    }

    [[nodiscard]] std::map<size_t, Employee> getEmployees() const {
        return employees;
    }


    void printDepartament() const;

    bool operator<(const Department &rhs) const {
        return employeesAmount < rhs.employeesAmount;
    }

private:
    std::string departmentName;
    size_t employeesAmount = 0;
    double averageSalary = 0;
    std::map<size_t, Employee> employees;
    double curTotalSalary = 0;
};

#endif //AVSOFT_TASK_DEPARTMENTS_HPP
