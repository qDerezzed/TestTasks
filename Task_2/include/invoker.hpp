#ifndef AVSOFT_TASK_INVOKER_HPP
#define AVSOFT_TASK_INVOKER_HPP

#include <stack>
#include <utility>

#include "departments.hpp"

class Command {
protected:
    Department *department = nullptr;
    std::map<std::string, Department> *departments = nullptr;
public:
    virtual ~Command() = default;

    virtual void Execute() = 0;

    virtual void unExecute() = 0;

    void setDepartment(Department *_department) {
        department = _department;
    }

    void setDepartments(std::map<std::string, Department> *_departments) {
        departments = _departments;
    }
};

class AddEmployeeCommand : public Command {
    Employee employee;
    size_t id{};
public:
    explicit AddEmployeeCommand(Employee inputEmployee) : employee(std::move(inputEmployee)) {}

    void Execute() override {
        id = department->addEmployee(employee);
    }

    void unExecute() override {
        department->deleteEmployee(id);
    }
};

class AddDepartmentCommand : public Command {
    std::string departmentName;
public:
    explicit AddDepartmentCommand(std::string inputDepartmentName) : departmentName(std::move(inputDepartmentName)) {}

    void Execute() override {
        departments->emplace(departmentName, departmentName);
    }

    void unExecute() override {
        departments->erase(departmentName);
    }
};

class EditEmployeeFunctionCommand : public Command {
    std::string newEmployeeFunction;
    std::string oldEmployeeFunction;
    size_t id{};
public:
    EditEmployeeFunctionCommand(size_t inputId, std::string employeeFunction) : newEmployeeFunction(
            std::move(employeeFunction)), id(inputId) {}

    void Execute() override;

    void unExecute() override;
};

class EditEmployeeSalaryCommand : public Command {
    int newEmployeeSalary{};
    int oldEmployeeSalary{};
    size_t id{};
public:
    EditEmployeeSalaryCommand(size_t inputId, int employeeSalary) : newEmployeeSalary(employeeSalary),
                                                                    id(inputId) {}

    void Execute() override;

    void unExecute() override;
};

class EditDepartmentCommand : public Command {
    std::string newDepartmentName;
    std::string oldDepartmentName;
public:
    explicit EditDepartmentCommand(std::string inputDepartmentName) : newDepartmentName(
            std::move(inputDepartmentName)) {}

    void Execute() override;

    void unExecute() override;
};

class DeleteEmployeeCommand : public Command {
    size_t id{};
    std::string depName;
    std::pair<size_t, Employee> employee;
public:
    explicit DeleteEmployeeCommand(size_t inputId) : id(inputId) {}

    void Execute() override;

    void unExecute() override {
        departments->at(depName).addEmployee(employee);
    }
};

class DeleteDepartmentCommand : public Command {
    Department deleteDepartment;
    std::string depName;
public:
    explicit DeleteDepartmentCommand(std::string inputDepName) : depName(std::move(inputDepName)) {}

    void Execute() override {
        deleteDepartment = departments->at(depName);
        departments->erase(depName);
    }

    void unExecute() override {
        departments->emplace(depName, deleteDepartment);
    }
};

class Invoker {
    std::stack<Command *> DoneCommands;
    Department *department = nullptr;
    std::map<std::string, Department> *departments = nullptr;
    Command *command = nullptr;
public:

    void setDepartment(Department *_department) {
        department = _department;
    }

    void setDepartments(std::map<std::string, Department> *_departments) {
        departments = _departments;
    }

    void AddEmployee(const Employee &employee);

    void AddDepartment(const std::string &inputDepartmentName);

    void EditEmployeeFunction(size_t inputId, const std::string &employeeFunction);

    void EditEmployeeSalary(size_t inputId, int salary);

    void EditDepartment(const std::string &inputDepName);

    void DeleteEmployee(size_t id);

    void DeleteDepartment(const std::string &depName);

    void Undo();
};

#endif //AVSOFT_TASK_INVOKER_HPP
