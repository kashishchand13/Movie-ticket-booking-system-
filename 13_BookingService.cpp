#ifndef BOOKINGSERVICE_CPP
#define BOOKINGSERVICE_CPP

#include <string>
#include <vector>
#include <iostream>
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
using namespace std;

// Responsibility: orchestrate the booking flow end to end (create booking,
// take payment, print ticket, cancel). Must NOT itself calculate price or
// print a ticket - it asks PriceCalculator / TicketPrinter to do that
// (Dependency Inversion: it depends on the Payment abstraction only).
class BookingService {
private:
    vector<Booking*> bookings;     // composition: BookingService owns every Booking it creates
    static int nextBookingId;      // static member: shared counter for unique ids
    PriceCalculator priceCalculator;
    TicketPrinter ticketPrinter;

public:
    ~BookingService() {
        for (Booking* booking : bookings) delete booking;
    }

    // FR4: reject the whole booking if any seat is already booked / invalid.
    // this() is unused here on purpose - no ambiguity to resolve in this method.
    Booking* createBooking(Show* show, const vector<string>& seatNumbers, Customer customer) {
        vector<ShowSeat*> selectedSeats;
        for (const string& seatNumber : seatNumbers) {
            ShowSeat* showSeat = show->findShowSeat(seatNumber);
            bool seatExists = showSeat != nullptr;
            if (!seatExists) {
                cout << "  Seat " << seatNumber << " does not exist on this screen.\n";
                return nullptr;
            }
            if (!showSeat->isAvailable()) {
                cout << "  Seat " << seatNumber << " is already BOOKED. Booking rejected.\n";
                return nullptr;
            }
            selectedSeats.push_back(showSeat);
        }

        double total = priceCalculator.calculateTotal(selectedSeats);
        string bookingId = generateBookingId();
        Booking* booking = new Booking(bookingId, show, customer, selectedSeats, total);
        bookings.push_back(booking);
        return booking;
    }

    // Compile-time (method) overloading: a single-seat convenience wrapper
    // around the vector<string> version above.
    Booking* createBooking(Show* show, const string& seatNumber, Customer customer) {
        vector<string> singleSeat;
        singleSeat.push_back(seatNumber);
        return createBooking(show, singleSeat, customer);
    }

    // FR6: exactly one payment method; failure must not confirm the booking.
    bool makePayment(Booking* booking, Payment* payment) {
        booking->attachPayment(payment);
        bool paymentSucceeded = payment->pay(booking->getTotalAmount());
        if (!paymentSucceeded) {
            booking->fail();
            return false;
        }
        for (ShowSeat* seat : booking->getSeats()) seat->book();
        booking->confirm();
        return true;
    }

    void printTicket(const Booking& booking) {
        ticketPrinter.printTicket(booking);
    }

    // FR8: cancelling releases the seats for that show only.
    bool cancelBooking(const string& bookingId) {
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr) {
            cout << "  No booking found with id " << bookingId << "\n";
            return false;
        }
        if (booking->getStatus() != BookingStatus::CONFIRMED) {
            cout << "  Booking " << bookingId << " is not CONFIRMED, cannot cancel.\n";
            return false;
        }
        for (ShowSeat* seat : booking->getSeats()) seat->release();
        booking->cancel();
        cout << "  Booking " << bookingId << " CANCELLED. Seats released.\n";
        return true;
    }

    Booking* findBooking(const string& bookingId) {
        for (Booking* booking : bookings) {
            if (booking->getBookingId() == bookingId) return booking;
        }
        return nullptr;
    }

    const vector<Booking*>& getAllBookings() const { return bookings; }

private:
    string generateBookingId() {
        string id = "BK" + to_string(nextBookingId);
        nextBookingId++;
        return id;
    }
};

int BookingService::nextBookingId = 1001;

#endif
