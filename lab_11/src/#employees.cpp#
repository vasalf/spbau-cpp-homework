#include "employees.h"

#include <cstring>
#include <algorithm>
#include <string>

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// Reads a string from binary file
static void input_string(std::istream& is, char*& where) {
    std::streampos start_from = is.tellg();
    std::size_t size = 1;
    while (is.get() != '\0')
        size++;
    where = new char[size];
    is.seekg(start_from);
    is.read(where, size);
}

Employee::~Employee() {}

std::istream& operator>>(std::istream& is, Employee& emp) {
    emp.input(is);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Employee& emp) {
    emp.print(os);
    return os;
}

void Developer::print(std::ostream& os) const {
    int32_t type = 1;
    os.write((char*)&type, sizeof(int32_t));
    os.write(_name, std::strlen(_name) + 1);
    os.write((char*)&_base_salary, sizeof(int32_t));
    os.write((char*)&_has_bonus, sizeof(bool));
}

void Developer::input(std::istream& is) {
    if (_name)
        this->~Developer();
    input_string(is, _name);
    is.read((char*)&_base_salary, sizeof(int32_t));
    is.read((char*)&_has_bonus, sizeof(bool));
}

void Developer::print_console(std::ostream& os) const {
    os << "Developer" << std::endl;
    os << "Name: " << _name << std::endl;
    os << "Base Salary: " << _base_salary << std::endl;
    os << "Has bonus: " << (_has_bonus ? '+' : '-');
}

void Developer::input_console(std::istream& is) {
    std::string s;
    is >> s;
    _name = new char[s.size() + 1];
    strcpy(_name, s.c_str());
    is >> _base_salary >> _has_bonus;
}

Developer::Developer() {
    _name = nullptr;
}

Developer::Developer(const Developer& other) {
    if (other._name) {
        _name = new char[strlen(other._name)];
        strcpy(_name, other._name);
    }
    _base_salary = other._base_salary;
    _has_bonus = other._has_bonus;
}

Developer& Developer::operator=(Developer other) {
    if (_name)
        this->~Developer();
    std::swap(_name, other._name);
    std::swap(_base_salary, other._base_salary);
    std::swap(_has_bonus, other._has_bonus);
    return *this;
}

int Developer::salary() const {
    int salary = _base_salary;
    if (_has_bonus) { salary += 1000; }
    return salary;
}

Developer::~Developer() {
    delete[] _name;
    _name = nullptr;
}

void SalesManager::print(std::ostream& os) const {
    int32_t type = 2;
    os.write((char*)&type, sizeof(int32_t));
    os.write(_name, std::strlen(_name) + 1);
    os.write((char*)&_base_salary, sizeof(int32_t));
    os.write((char*)&_sold_nm, sizeof(int32_t));
    os.write((char*)&_price, sizeof(int32_t));
}

void SalesManager::input(std::istream& is) {
    if (_name)
        this->~SalesManager();
    input_string(is, _name);
    is.read((char*)&_base_salary, sizeof(int32_t));
    is.read((char*)&_sold_nm, sizeof(int32_t));
    is.read((char*)&_price, sizeof(int32_t));
}

SalesManager::SalesManager() {
    _name = nullptr;
}

SalesManager::SalesManager(const SalesManager& other) {
    if (other._name) {
        _name = new char[strlen(other._name) + 1];
        strcpy(_name, other._name);
    }
    _base_salary = other._base_salary;
    _sold_nm = other._sold_nm;
    _price = other._price;
}

SalesManager& SalesManager::operator=(SalesManager other) {
    if (_name)
        this->~SalesManager();
    std::swap(_name, other._name);
    std::swap(_base_salary, other._base_salary);
    std::swap(_sold_nm, other._sold_nm);
    std::swap(_price, other._price);
    return *this;
}

void SalesManager::print_console(std::ostream& os) const {
    os << "Sales Manager" << std::endl;
    os << "Name: " << _name << std::endl;
    os << "Base Salary: " << _base_salary << std::endl;
    os << "Sold Items: " << _sold_nm << std::endl;
    os << "Item price: " << _price;
}

void SalesManager::input_console(std::istream& is) {
    std::string s;
    is >> s;
    _name = new char[s.size() + 1];
    strcpy(_name, s.c_str());
    is >> _base_salary >> _sold_nm >> _price;
}

int SalesManager::salary() const {
    return _base_salary + _sold_nm * _price * 0.01;
}

SalesManager::~SalesManager() {
    delete[] _name;
    _name = 0;
}

EmployeesArray::EmployeesArray() {
    _size = 0;
    _capacity = 1;
    _employees = new Employee*[1];
}

EmployeesArray::EmployeesArray(const EmployeesArray& other) {
    _size = other._size;
    _capacity = other._capacity;
    _employees = new Employee*[_capacity];
    std::copy(other._employees, other._employees + other._size, _employees);
}

EmployeesArray& EmployeesArray::operator=(EmployeesArray other) {
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    std::swap(_employees, other._employees);
    return *this;
}

void EmployeesArray::add(Employee* e) {
    if (_size == _capacity) {
        _capacity *= 2;
        Employee** tmp;
        tmp = new Employee*[_capacity];
        std::copy(_employees, _employees + _size, tmp);
        delete[] _employees;
        _employees = tmp;
    }
    _employees[_size++] = e;
}

int EmployeesArray::total_salary() const {
    int ret = 0;
    for (std::size_t i = 0; i != _size; i++)
        ret += _employees[i]->salary();
    return ret;
}

void EmployeesArray::print_console(std::ostream& os) const {
    for (std::size_t i = 0; i != _size; i++) {
        os << i + 1 << ": ";
        _employees[i]->print_console(os);
        os << std::endl;
    }
    os << "== Total salary: " << total_salary() << std::endl;
}

std::istream& operator>>(std::istream& is, EmployeesArray& emp) {
    emp = EmployeesArray();
    int32_t num;
    is.read((char*)&num, sizeof(int32_t));
    for (int32_t i = 0; i != num; i++) {
        Employee* n;
        int32_t type;
        is.read((char*)&type, sizeof(int32_t));
        if (type == 1) {
            n = new Developer();
            is >> *n;
        } else {
            n = new SalesManager();
            is >> *n;
        }
        emp.add(n);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const EmployeesArray& emp) {
    int32_t sz = emp._size;
    os.write((char*)&sz, sizeof(int32_t));
    for (std::size_t i = 0; i != emp._size; i++) {
        os << *emp._employees[i];
    }
    return os;
}

EmployeesArray EmployeesArray::operator+(const EmployeesArray& other) const {
    EmployeesArray ret = *this;
    for (size_t i = 0; i != other._size; i++) {
        ret.add(other._employees[i]);
    }
    return ret;
}

void EmployeesArray::delete_all() const {
    for (size_t i = 0; i != _size; i++)
        delete _employees[i];
}

EmployeesArray::~EmployeesArray() {
    delete[] _employees;
}

