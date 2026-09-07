#ifndef BOOKING_CPP
#define BOOKING_CPP

#include <string>
#include <vector>
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"
using namespace std;

enum class BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

// Responsibility: hold booking data (id, show, seats, customer, amount, status).
// Must NOT do: must not print a ticket, must not calculate price, must not
// decide which payment method to use - those belong to other classes.
class Booking {
private:
    string bookingId;
    Show* show;
    Customer customer;
    vector<ShowSeat*> seats;       // aggregation: booking references seats it does not own
    double totalAmount;
    BookingStatus status;
    Payment* payment;              // composition: this Payment exists only for this booking

public:
    Booking(string bookingId, Show* show, Customer customer, vector<ShowSeat*> seats, double totalAmount)
        : bookingId(bookingId), show(show), customer(customer), seats(seats),
          totalAmount(totalAmount), status(BookingStatus::PENDING), payment(nullptr) {}

    ~Booking() { delete payment; }

    string getBookingId() const { return bookingId; }
    Show* getShow() const { return show; }
    const Customer& getCustomer() const { return customer; }
    const vector<ShowSeat*>& getSeats() const { return seats; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }

    void attachPayment(Payment* newPayment) { payment = newPayment; }

    void confirm() { setStatus(BookingStatus::CONFIRMED); }
    void fail() { setStatus(BookingStatus::FAILED); }
    void cancel() { setStatus(BookingStatus::CANCELLED); }

private:
    // "this" keyword: disambiguates the member from the parameter of the
    // same name, the same way a Java setter would use this.status = status.
    void setStatus(BookingStatus status) { this->status = status; }
};

#endif
