#ifndef PAYMENT_CPP
#define PAYMENT_CPP

// Responsibility: the payment contract only. Must NOT know about Booking or seats.
// Abstraction: concrete payment methods must implement pay(); this class never does.
class Payment {
protected:
    double amount;

public:
    Payment(double amount) : amount(amount) {}
    virtual ~Payment() {}

    // Pure virtual -> every subclass MUST provide its own pay(); enables
    // runtime polymorphism when called through a Payment* / Payment&.
    virtual bool pay(double amountToPay) = 0;
};

#endif
