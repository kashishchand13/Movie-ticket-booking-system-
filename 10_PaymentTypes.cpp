#ifndef PAYMENTTYPES_CPP
#define PAYMENTTYPES_CPP

#include <string>
#include <iostream>
#include "09_Payment.cpp"
using namespace std;

// Each class below inherits Payment and implements pay() in its own way.
// FR6: a failed payment must not confirm the booking - pay() truthfully
// returns false on failure and does NOT touch any seat or booking state itself.

class UpiPayment : public Payment {
private:
    string upiId;

public:
    UpiPayment(double amount, string upiId) : Payment(amount), upiId(upiId) {}

    bool pay(double amountToPay) override {
        bool isValidUpiId = upiId.find('@') != string::npos;
        if (!isValidUpiId) {
            cout << "  [UPI] payment FAILED - invalid UPI id\n";
            return false;
        }
        cout << "  [UPI] Rs." << amountToPay << " paid successfully\n";
        return true;
    }
};

class CardPayment : public Payment {
private:
    string cardNumber;

public:
    CardPayment(double amount, string cardNumber) : Payment(amount), cardNumber(cardNumber) {}

    bool pay(double amountToPay) override {
        bool isValidCard = cardNumber.size() == 16;
        if (!isValidCard) {
            cout << "  [CARD] payment FAILED - invalid card number\n";
            return false;
        }
        cout << "  [CARD] Rs." << amountToPay << " paid successfully\n";
        return true;
    }
};

class CashPayment : public Payment {
private:
    double cashTendered;

public:
    CashPayment(double amount, double cashTendered) : Payment(amount), cashTendered(cashTendered) {}

    bool pay(double amountToPay) override {
        bool isEnoughCash = cashTendered >= amountToPay;
        if (!isEnoughCash) {
            cout << "  [CASH] payment FAILED - insufficient cash tendered\n";
            return false;
        }
        cout << "  [CASH] Rs." << amountToPay << " paid successfully\n";
        return true;
    }
};

#endif
