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

    virtual bool Execute() = 0;

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

    bool Execute() override {
        id = department->addEmployee(employee);
        return true;
    }

    void unExecute() override {
        department->deleteEmployee(id);
    }
};

class AddDepartmentCommand : public Command {
    std::string departmentName;
public:
    explicit AddDepartmentCommand(std::string inputDepartmentName) : departmentName(std::move(inputDepartmentName)) {}

    bool Execute() override {
        departments->emplace(departmentName, departmentName);
        return true;
    }

    void unExecute() override {
        departments->erase(departmentName);
    }
};

class EditEmployeeSurNameCommand : public Command {
    std::string newEmployeeSurName;
    std::string oldEmployeeSurName;
    size_t id{};
public:
    EditEmployeeSurNameCommand(size_t inputId, std::string employeeSurName) : newEmployeeSurName(
            std::move(employeeSurName)), id(inputId) {}

    bool Execute() override;

    void unExecute() override;
};

class EditEmployeeNameCommand : public Command {
    std::string newEmployeeName;
    std::string oldEmployeeName;
    size_t id{};
public:
    EditEmployeeNameCommand(size_t inputId, std::string employeeName) : newEmployeeName(
            std::move(employeeName)), id(inputId) {}

    bool Execute() override;

    void unExecute() override;
};

class EditEmployeeMiddleNameCommand : public Command {
    std::string newEmployeeMiddleName;
    std::string oldEmployeeMiddleName;
    size_t id{};
public:
    EditEmployeeMiddleNameCommand(size_t inputId, std::string employeeMiddleName) : newEmployeeMiddleName(
            std::move(employeeMiddleName)), id(inputId) {}

    bool Execute() override;

    void unExecute() override;
};

class EditEmployeeFunctionCommand : public Command {
    std::string newEmployeeFunction;
    std::string oldEmployeeFunction;
    size_t id{};
public:
    EditEmployeeFunctionCommand(size_t inputId, std::string employeeFunction) : newEmployeeFunction(
            std::move(employeeFunction)), id(inputId) {}

    bool Execute() override;

    void unExecute() override;
};

class EditEmployeeSalaryCommand : public Command {
    int newEmployeeSalary{};
    int oldEmployeeSalary{};
    size_t id{};
public:
    EditEmployeeSalaryCommand(size_t inputId, int employeeSalary) : newEmployeeSalary(employeeSalary),
                                                                    id(inputId) {}

    bool Execute() override;

    void unExecute() override;
};

class EditDepartmentCommand : public Command {
    std::string newDepartmentName;
    std::string oldDepartmentName;
public:
    explicit EditDepartmentCommand(std::string inputDepartmentName) : newDepartmentName(
            std::move(inputDepartmentName)) {}

    bool Execute() override;

    void unExecute() override;
};

class DeleteEmployeeCommand : public Command {
    size_t id{};
    std::string depName;
    std::pair<size_t, Employee> employee;
public:
    explicit DeleteEmployeeCommand(size_t inputId) : id(inputId) {}

    bool Execute() override;

    void unExecute() override {
        if (!depName.empty()) {
            departments->at(depName).addEmployee(employee);
        }
    }
};

class DeleteDepartmentCommand : public Command {
    Department deleteDepartment;
    std::string depName;
public:
    explicit DeleteDepartmentCommand(std::string inputDepName) : depName(std::move(inputDepName)) {}

    bool Execute() override {
        deleteDepartment = departments->at(depName);
        departments->erase(depName);
        return true;
    }

    void unExecute() override {
        departments->emplace(depName, deleteDepartment);
    }
};

class Invoker {
    std::stack<Command *> DoneCommands;
    std::stack<Command *> UndoCommands;
    Department *department = nullptr;
    std::map<std::string, Department> *departments = nullptr;
    Command *command = nullptr;

    void addToDoneCommands(Command *inputCommand) {
        DoneCommands.push(inputCommand);
        while (!UndoCommands.empty()) {
            delete UndoCommands.top();
            UndoCommands.pop();
        }
    }

public:

    Invoker() = default;

    ~Invoker() {
        while (!DoneCommands.empty()) {
            delete DoneCommands.top();
            DoneCommands.pop();
        }
        while (!UndoCommands.empty()) {
            delete UndoCommands.top();
            UndoCommands.pop();
        }
    }

    void setDepartment(Department *_department) {
        department = _department;
    }

    void setDepartments(std::map<std::string, Department> *_departments) {
        departments = _departments;
    }

    void AddEmployee(const Employee &employee);

    void AddDepartment(const std::string &inputDepartmentName);

    void EditEmployeeSurname(size_t inputId, const std::string &employeeSurname);

    void EditEmployeeMiddleName(size_t inputId, const std::string &employeeMiddleName);

    void EditEmployeeName(size_t inputId, const std::string &employeeName);

    void EditEmployeeFunction(size_t inputId, const std::string &employeeFunction);

    void EditEmployeeSalary(size_t inputId, int salary);

    void EditDepartment(const std::string &inputDepName);

    void DeleteEmployee(size_t id);

    void DeleteDepartment(const std::string &depName);

    void Undo();

    void Redo();
};

#endif //AVSOFT_TASK_INVOKER_HPP
