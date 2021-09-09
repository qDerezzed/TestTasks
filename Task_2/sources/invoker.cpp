#include "invoker.hpp"

bool EditEmployeeSurNameCommand::Execute() {
    for (auto &[depName, dep] : *departments) {
        oldEmployeeSurName = dep.setEmployeeSurName(id, newEmployeeSurName);
        if (!oldEmployeeSurName.empty()) {
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}

void EditEmployeeSurNameCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (!dep.setEmployeeSurName(id, oldEmployeeSurName).empty()) {
            break;
        }
    }
}

bool EditEmployeeNameCommand::Execute() {
    for (auto &[depName, dep] : *departments) {
        oldEmployeeName = dep.setEmployeeName(id, newEmployeeName);
        if (!oldEmployeeName.empty()) {
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}

void EditEmployeeNameCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (!dep.setEmployeeName(id, oldEmployeeName).empty()) {
            break;
        }
    }
}

bool EditEmployeeMiddleNameCommand::Execute() {
    for (auto &[depName, dep] : *departments) {
        oldEmployeeMiddleName = dep.setEmployeeMiddleName(id, newEmployeeMiddleName);
        if (!oldEmployeeMiddleName.empty()) {
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}

void EditEmployeeMiddleNameCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (!dep.setEmployeeMiddleName(id, oldEmployeeMiddleName).empty()) {
            break;
        }
    }
}

bool EditEmployeeFunctionCommand::Execute() {
    for (auto &[depName, dep] : *departments) {
        oldEmployeeFunction = dep.setEmployeeFunction(id, newEmployeeFunction);
        if (!oldEmployeeFunction.empty()) {
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}


void EditEmployeeFunctionCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (!dep.setEmployeeFunction(id, oldEmployeeFunction).empty()) {
            break;
        }
    }
}

bool EditEmployeeSalaryCommand::Execute() {
    for (auto &[departamentName, departament] : *departments) {
        if (departament.setEmployeeSalary(id, newEmployeeSalary, oldEmployeeSalary)) {
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}

void EditEmployeeSalaryCommand::unExecute() {
    for (auto &[depName, dep] : *departments) {
        if (dep.setEmployeeSalary(id, oldEmployeeSalary, newEmployeeSalary)) {
            break;
        }
    }
}

bool EditDepartmentCommand::Execute() {
    oldDepartmentName = department->getDepartmentName();
    department->setDepartmentName(newDepartmentName);
    auto nh = departments->extract(oldDepartmentName);
    nh.key() = newDepartmentName;
    departments->insert(move(nh));
    return true;
}

void EditDepartmentCommand::unExecute() {
    department->setDepartmentName(oldDepartmentName);
    auto nh = departments->extract(newDepartmentName);
    nh.key() = oldDepartmentName;
    departments->insert(move(nh));
}

bool DeleteEmployeeCommand::Execute() {
    for (auto &[departamentName, departament] : *departments) {
        if (departament.deleteEmployee(id, employee)) {
            depName = departamentName;
            return true;
        }
    }
    std::cout << "error: wrong id" << std::endl;
    return false;
}

void Invoker::AddEmployee(const Employee &employee) {
    command = new AddEmployeeCommand(employee);
    command->setDepartment(department);
    command->Execute();
    addToDoneCommands(command);
}

void Invoker::AddDepartment(const std::string &inputDepartmentName) {
    command = new AddDepartmentCommand(inputDepartmentName);
    command->setDepartments(departments);
    command->Execute();
    addToDoneCommands(command);
}

void Invoker::EditEmployeeSurname(size_t inputId, const std::string &employeeSurname) {
    command = new EditEmployeeSurNameCommand(inputId, employeeSurname);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::EditEmployeeName(size_t inputId, const std::string &employeeName) {
    command = new EditEmployeeNameCommand(inputId, employeeName);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::EditEmployeeMiddleName(size_t inputId, const std::string &employeeMiddleName) {
    command = new EditEmployeeMiddleNameCommand(inputId, employeeMiddleName);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::EditEmployeeFunction(size_t inputId, const std::string &employeeFunction) {
    command = new EditEmployeeFunctionCommand(inputId, employeeFunction);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::EditEmployeeSalary(size_t inputId, int salary) {
    command = new EditEmployeeSalaryCommand(inputId, salary);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::EditDepartment(const std::string &inputDepName) {
    command = new EditDepartmentCommand(inputDepName);
    command->setDepartment(department);
    command->setDepartments(departments);
    command->Execute();
    addToDoneCommands(command);
}

void Invoker::DeleteEmployee(size_t id) {
    command = new DeleteEmployeeCommand(id);
    command->setDepartment(department);
    command->setDepartments(departments);
    if (command->Execute()) {
        addToDoneCommands(command);
    }
}

void Invoker::DeleteDepartment(const std::string &depName) {
    command = new DeleteDepartmentCommand(depName);
    command->setDepartments(departments);
    command->Execute();
    addToDoneCommands(command);
}

void Invoker::Undo() {
    if (DoneCommands.empty()) {
        std::cout << "There is nothing to undo!" << std::endl;
    } else {
        command = DoneCommands.top();
        DoneCommands.pop();
        command->unExecute();
        UndoCommands.push(command);
        std::cout << "Undo is succes" << std::endl;
    }
}

void Invoker::Redo() {
    if (UndoCommands.empty()) {
        std::cout << "There is nothing to redo!" << std::endl;
    } else {
        command = UndoCommands.top();
        UndoCommands.pop();
        command->Execute();
        DoneCommands.push(command);
        std::cout << "Redo is succes" << std::endl;
    }
}
