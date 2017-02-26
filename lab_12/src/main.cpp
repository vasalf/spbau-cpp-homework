#include <iostream>
#include <string>

#include "my_vector.h"

using namespace std;

class Product {
public:
    Product(const string& name, int quantity, double price) {
        name_ = name;
        quantity_ = quantity;
        price_ = price;
    }
    Product() {}
    friend ostream& operator<<(ostream& os, const Product& p);
private:
    string name_;
    int quantity_;
    double price_;
};

ostream& operator<<(ostream& os, const Product& p) {
    os << p.name_ << " " << p.quantity_ << " " << p.price_;
    return os;
}

template<class T>
void print(const my_vector<T>& v) {
    std::cout << v << std::endl;
}

template<class T>
void test_my_vector(T a, T b) {
    my_vector<T> vec;
    vec.resize(3);
    cout << vec.size() << " " << vec.capacity() << endl;
    vec.reserve(6);
    cout << vec.size() << " " << vec.capacity() << endl;
    vec[0] = a;
    vec[1] = b;
    vec[2] = a;
    vec.push_back(b);
    cout << vec.size() << " " << vec.capacity() << endl;
    vec.push_back(a);
    cout << vec.size() << " " << vec.capacity() << endl;
    vec.pop_back();
    cout << vec.size() << " " << vec.capacity() << endl;
    print(vec);
}

int main() {
    test_my_vector<int>(5, 10);
    test_my_vector<Product>(Product("asdf", 4, 12.0), Product("qwe", -1, 7.5));

    return 0;
}

