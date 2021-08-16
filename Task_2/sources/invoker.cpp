#include "invoker.hpp"

void EditEmployeeFunctionCommand::Execute() {
    for (auto &[depName, dep] : *departments) {
        oldEmployeeFunction = dep.setEmployeeFunction(id, newEmployeeFunction);
        if (!oldEmployeeFunction.empty()) {
            break;
        }
    }
}

void EditEmployeeFunctionCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (!dep.setEmployeeFunction(id, oldEmployeeFunction).empty()) {
            break;
        }
    }
}

void EditEmployeeSalaryCommand::Execute() {
    for (auto &[departamentName, departament] : *departments) {
        if (departament.setEmployeeSalary(id, newEmployeeSalary, oldEmployeeSalary)) {
            break;
        }
    }
}

void EditEmployeeSalaryCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (dep.setEmployeeSalary(id, oldEmployeeSalary, newEmployeeSalary)) {
            break;
        }
    }
}

void EditDepartmentCommand::Execute() {
    oldDepartmentName = department->getDepartmentName();
    department->setDepartmentName(newDepartmentName);
    auto nh = departments->extract(oldDepartmentName);
    nh.key() = newDepartmentName;
    departments->insert(move(nh));
}

void EditDepartmentCommand::unExecute() {
    department->setDepartmentName(oldDepartmentName);
    auto nh = departments->extract(newDepartmentName);
    nh.key() = oldDepartmentName;
    departments->insert(move(nh));
}

void DeleteEmployeeCommand::Execute() {
    for (auto &[departamentName, departament] : *departments) {
        if (departament.deleteEmployee(id, employee)) {
            depName = departamentName;
            break;
        }
    }
}

void Invoker::AddEmployee(const Employee &employee) {
    command = new AddEmployeeCommand(employee);
    command->setDepartment(department);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::AddDepartment(const std::string &inputDepartmentName) {
    command = new AddDepartmentCommand(inputDepartmentName);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::EditEmployeeFunction(size_t inputId, const std::string &employeeFunction) {
    command = new EditEmployeeFunctionCommand(inputId, employeeFunction);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::EditEmployeeSalary(size_t inputId, int salary) {
    command = new EditEmployeeSalaryCommand(inputId, salary);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::EditDepartment(const std::string &inputDepName) {
    command = new EditDepartmentCommand(inputDepName);
    command->setDepartment(department);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::DeleteEmployee(size_t id) {
    command = new DeleteEmployeeCommand(id);
    command->setDepartment(department);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::DeleteDepartment(const std::string &depName) {
    command = new DeleteDepartmentCommand(depName);
    command->setDepartments(departments);
    command->Execute();
    DoneCommands.push(command);
}

void Invoker::Undo() {
    if (DoneCommands.empty()) {
        std::cout << "There is nothing to undo!" << std::endl;
    } else {
        command = DoneCommands.top();
        DoneCommands.pop();
        command->unExecute();
        std::cout << "Undo is succes" << std::endl;
        delete command;
    }
}
