#include <gtest/gtest.h>

#include "departments.hpp"
#include "parser.hpp"
#include "invoker.hpp"

TEST(Parser, test) {
    Parser parser("../Task_2/data/departments_data.xml");
    std::map<std::string, Department> departments = parser.parse();

    EXPECT_EQ(departments.size(), 3);
}

TEST(Departments, test) {
    std::map<size_t, Employee> employees = {{0, {"a", "b", "c", "d", 100}},
                                            {1, {"q", "w", "e", "c", 200}}};
    Department department("qwe", employees.size(), 150, employees);
    EXPECT_EQ(department.getEmployeesAmount(), 2);
    EXPECT_EQ(department.getAverageSalary(), 150);

    department.setDepartmentName("web");
    EXPECT_EQ(department.getDepartmentName(), "web");

    department.setEmployeeName(0, "Ivan");
    EXPECT_EQ(department.getEmployees()[0].name, "Ivan");

    int oldValue = 0;
    department.setEmployeeSalary(0, 200, oldValue);
    EXPECT_EQ(department.getAverageSalary(), 200);

    department.addEmployee({"z", "x", "c", "v", 500});
    EXPECT_EQ(department.getAverageSalary(), 300);
    EXPECT_EQ(department.getEmployeesAmount(), 3);
    EXPECT_EQ(department.getEmployees()[2].surname, "z");

    department.deleteEmployee(0);
    EXPECT_EQ(department.getEmployeesAmount(), 2);
    EXPECT_EQ(department.getAverageSalary(), 350);

    department.addEmployee({"p", "x", "c", "v", 500});
    EXPECT_EQ(department.getAverageSalary(), 400);
    EXPECT_EQ(department.getEmployeesAmount(), 3);
    EXPECT_EQ(department.getEmployees()[3].surname, "p");
}

TEST(Invoker, test) {
    std::map<size_t, Employee> employees = {{0, {"a", "b", "c", "d", 100}},
                                            {1, {"q", "w", "e", "c", 200}}};
    Department department("qwe", employees.size(), 150, employees);

    Invoker inv;
    inv.setDepartment(&department);

    inv.AddEmployee({"z", "x", "c", "v", 500});
    EXPECT_EQ(department.getEmployees()[2].surname, "z");

    inv.Undo();
    EXPECT_EQ(department.getEmployeesAmount(), 2);

    std::map<std::string, Department> departaments;
    departaments.insert({department.getDepartmentName(), department});
    departaments.emplace("asd", "asd");

    inv.setDepartments(&departaments);
    inv.AddDepartment("newDep");
    EXPECT_TRUE(departaments.contains("newDep"));
    inv.Undo();
    EXPECT_FALSE(departaments.contains("newDep"));

    inv.EditEmployeeFunction(0, "developer");
    EXPECT_EQ(departaments["qwe"].getEmployees()[0].function, "developer");
    inv.Undo();
    EXPECT_EQ(departaments["qwe"].getEmployees()[0].function, "d");

    inv.EditEmployeeSalary(0, 123);
    EXPECT_EQ(departaments["qwe"].getEmployees()[0].salary, 123);
    inv.Undo();
    EXPECT_EQ(departaments["qwe"].getEmployees()[0].salary, 100);

    inv.setDepartment(&department);
    inv.EditDepartment("web");
    EXPECT_EQ(department.getDepartmentName(), "web");
    inv.Undo();
    EXPECT_EQ(department.getDepartmentName(), "qwe");

    inv.DeleteEmployee(0);
    for (const auto &dep : departaments) {
        EXPECT_FALSE(dep.second.getEmployees().contains(0));
    }
    inv.Undo();
    EXPECT_TRUE(departaments["qwe"].getEmployees().contains(0));

    inv.DeleteDepartment("asd");
    EXPECT_FALSE(departaments.contains("asd"));
    inv.Undo();
    EXPECT_TRUE(departaments.contains("asd"));

}