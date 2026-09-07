#ifndef CUSTOMER_CPP
#define CUSTOMER_CPP

#include <string>
using namespace std;

// Responsibility: a customer's name and phone number. Nothing about bookings.
class Customer {
private:
    string name;
    string phone;

public:
    Customer(string name, string phone) : name(name), phone(phone) {}

    // Compile-time (constructor) overloading: a walk-in customer who didn't
    // share a phone number. Delegates to the two-argument constructor.
    Customer(string name) : Customer(name, "N/A") {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
};

#endif
