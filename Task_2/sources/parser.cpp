#include "parser.hpp"

void Parser::saveChanges(const std::map<std::string, Department> &departments) const {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    doc.Clear();
    doc.InsertFirstChild(doc.NewDeclaration());

    auto *xmlDepartments = doc.InsertEndChild(doc.NewElement("departments"));
    for (const auto &department : departments) {
        tinyxml2::XMLElement *xmlEmployees = doc.NewElement("employments");
        for (const auto &employee : department.second.getEmployees()) {
            tinyxml2::XMLElement *xmlEmployment = doc.NewElement("employment");
            xmlEmployment->InsertNewChildElement("surname")->InsertNewText(employee.second.surname.c_str());
            xmlEmployment->InsertNewChildElement("name")->InsertNewText(employee.second.name.c_str());
            xmlEmployment->InsertNewChildElement("middleName")->InsertNewText(employee.second.middleName.c_str());
            xmlEmployment->InsertNewChildElement("function")->InsertNewText(employee.second.function.c_str());
            xmlEmployment->InsertNewChildElement("salary")->SetText(employee.second.salary);
            xmlEmployees->InsertEndChild(xmlEmployment);
        }
        tinyxml2::XMLElement *xmlDepartment = doc.NewElement("department");
        xmlDepartment->SetAttribute("name", department.first.c_str());
        xmlDepartment->InsertEndChild(xmlEmployees);
        xmlDepartments->InsertEndChild(xmlDepartment);
    }
    doc.SaveFile(fileName.c_str());
}

std::map<std::string, Department> Parser::parse() {
    std::map<std::string, Department> departments;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    for (tinyxml2::XMLElement *department = doc.FirstChildElement("departments")->FirstChildElement("department");
         department;
         department = department->NextSiblingElement()) {
        Department departmentObject(department->FirstAttribute()->Value());
        for (tinyxml2::XMLElement *employment = department->FirstChildElement("employments")->FirstChildElement(
                "employment");
             employment;
             employment = employment->NextSiblingElement()) {
            Employee employee{employment->FirstChildElement("surname")->GetText(),
                              employment->FirstChildElement("name")->GetText(),
                              employment->FirstChildElement("middleName")->GetText(),
                              employment->FirstChildElement("function")->GetText(),
                              employment->FirstChildElement("salary")->UnsignedText()};
            departmentObject.addEmployee(employee);
        }
        departments.insert({departmentObject.getDepartmentName(), departmentObject});
    }
    return departments;
}