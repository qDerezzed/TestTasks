#ifndef AVSOFT_TASK_PARSER_HPP
#define AVSOFT_TASK_PARSER_HPP

#include <iostream>
#include <tinyxml2.h>

#include "departments.hpp"

class Parser {
public:
    Parser() = default;

    Parser &operator=(const Parser &) = delete;

    ~Parser() = default;

    void readInputFileName() {
        std::cout << "Enter the path to the xml file: ";
        std::getline(std::cin, fileName);
    }

    static void printDepartamentsTree(const std::map<std::string, Department> &departments) {
        std::cout << "departaments" << std::endl << "|" << std::endl;
        for (const auto &[departamentName, departament] : departments) {
            std::cout << "+-" << departamentName << " - employees_amount: " << departament.getEmployeesAmount()
                      << " average_salary: " << departament.getAverageSalary() << std::endl << "  |" << std::endl;
            departament.printDepartament();
        }
    }

    void saveChanges(const std::map<std::string, Department> &departments) const;

    std::map<std::string, Department> parse();

private:
    std::string fileName;
};

#endif //AVSOFT_TASK_PARSER_HPP
