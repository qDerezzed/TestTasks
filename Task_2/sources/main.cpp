#include <algorithm>

#include "parser.hpp"
#include "invoker.hpp"
#include "menu.hpp"

Invoker inv;

int main() {
    Parser parser;
    parser.readInputFileName();
    std::map<std::string, Department> departments = parser.parse();
    inv.setDepartments(&departments);

    Parser::printDepartamentsTree(departments);
    Menu menu(&departments);
    size_t numMenu;
    do {
        Menu::printMenu();
        numMenu = Menu::getVariant(1, 11);
        switch (numMenu) {
            case 1:
                Parser::printDepartamentsTree(departments);
                break;
            case 2:
                menu.addEmployee();
                break;
            case 3:
                menu.addDepartament();
                break;
            case 4:
                menu.editEmployee();
                break;
            case 5:
                menu.editDepartment();
                break;
            case 6:
                menu.deleteEmployee();
                break;
            case 7:
                menu.deleteDepartment();
                break;
            case 8:
                inv.Undo();
                break;
            case 9:
                inv.Redo();
                break;
            case 10: {
                std::string fileName = Menu::getValidFilePath();
                if (fileName.empty()) {
                    break;
                }
                Parser::saveChanges(fileName, departments);
                std::cout << "the changes were successfully saved in the XML file" << std::endl;
                break;
            }
            case 11:
                std::cout << "Do you want to save the changes in the original XML file?" << std::endl << "1. Yes"
                          << std::endl << "2. No"
                          << std::endl << "selected action: " << std::endl
                          << "$ ";
                if (Menu::getVariant(1, 2) == 1) {
                    Parser::saveChanges(parser.getFileName(), departments);
                }
                return 0;
            default:
                return 1;
        }
    } while (numMenu != 11);

    return 0;
}
