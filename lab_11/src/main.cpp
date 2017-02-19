#include "employees.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    EmployeesArray cur;
    while (true) {
        string command;
        cin >> command;
        if (command == "load") {
            string filename;
            cin >> filename;
            ifstream file(filename, ios::binary);
            EmployeesArray new_employees;
            file >> new_employees;
            cur = cur + new_employees;
        } else if (command == "list") {
            cur.print_console(cout);
        } else if (command == "add") {
            int type;
            cin >> type;
            Employee *person;
            if (type == 1)
                person = new Developer();
            else 
                person = new SalesManager();
            person->input_console(cin);
            cur.add(person);
        } else if (command == "save") {
            string filename;
            cin >> filename;
            ofstream file(filename, ios::binary);
            file << cur;
        } else {
            break;
        }
    }
    cur.delete_all();
    return 0;
}
