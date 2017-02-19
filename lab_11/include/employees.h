#ifndef __LAB11_EMPLOYEES_H_INCLUDED
#define __LAB11_EMPLOYEES_H_INCLUDED

#include <cstdint>
#include <cstddef>
#include <istream>
#include <ostream>

class Employee {
protected:
    virtual void print(std::ostream& os) const = 0;
    virtual void input(std::istream& is) = 0;
public:
    virtual void print_console(std::ostream& os) const = 0;
    virtual void input_console(std::istream& is) = 0;
    virtual int salary() const = 0;
    friend std::istream& operator>>(std::istream& is, Employee& emp);
    friend std::ostream& operator<<(std::ostream& os, const Employee& emp);
    virtual ~Employee();
};

class Developer : public Employee {
protected:
    void print(std::ostream& os) const;
    void input(std::istream& is);
public:
    Developer();
    Developer(const Developer& other);
    Developer& operator=(Developer other);
    void print_console(std::ostream& os) const;
    void input_console(std::istream& is);
    int salary() const;
    ~Developer();
private:
    char *_name; // std::string is for dweebs! we will use pointers and write copy constructors and assignment operator for each new class!
    int32_t _base_salary;
    bool _has_bonus;
};

class SalesManager : public Employee {
protected:
    void print(std::ostream& os) const;
    void input(std::istream& is);
public:
    SalesManager();
    SalesManager(const SalesManager& other);
    SalesManager& operator=(SalesManager other);
    void print_console(std::ostream& os) const;
    void input_console(std::istream& is);
    int salary() const;
    ~SalesManager();
private:
    char *_name;
    int32_t _base_salary;
    int32_t _sold_nm, _price;
};

// no deriving from std::vector, no
class EmployeesArray {
public:
    EmployeesArray();
    EmployeesArray(const EmployeesArray& other);
    EmployeesArray & operator=(EmployeesArray other);
    void add(Employee *e);
    int total_salary() const;
    void print_console(std::ostream& os) const;
    friend std::istream& operator>>(std::istream& is, EmployeesArray& emp);
    friend std::ostream& operator<<(std::ostream& os, const EmployeesArray& emp);
    EmployeesArray operator+(const EmployeesArray& other) const;
    void delete_all() const;
    ~EmployeesArray();
private:
    size_t _size;
    size_t _capacity;
    Employee **_employees;
};

#endif
