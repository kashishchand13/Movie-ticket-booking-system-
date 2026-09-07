#ifndef TICKETPRINTER_CPP
#define TICKETPRINTER_CPP

#include <iostream>
#include "08_Booking.cpp"
using namespace std;

// Responsibility: format and print a ticket. Must NOT change booking status,
// price anything, or touch seat state - printing only.
class TicketPrinter {
public:
    void printTicket(const Booking& booking) const {
        cout << "\n  ================ TICKET ================\n";
        cout << "  Booking ID : " << booking.getBookingId() << "\n";
        cout << "  Movie      : " << booking.getShow()->getMovie()->getTitle() << "\n";
        cout << "  Screen     : Screen-" << booking.getShow()->getScreen()->getScreenNumber()
             << "   " << booking.getShow()->getStartTime() << "\n";
        cout << "  Seats      : " << seatListToString(booking) << "\n";
        cout << "  Amount     : Rs." << booking.getTotalAmount()
             << "        Status: " << statusToString(booking.getStatus()) << "\n";
        cout << "  =========================================\n";
    }

private:
    string seatListToString(const Booking& booking) const {
        string result;
        const vector<ShowSeat*>& seats = booking.getSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            result += seats[i]->getSeat()->getSeatNumber();
            if (i + 1 < seats.size()) result += ", ";
        }
        return result;
    }

    string statusToString(BookingStatus status) const {
        if (status == BookingStatus::CONFIRMED) return "CONFIRMED";
        if (status == BookingStatus::CANCELLED) return "CANCELLED";
        if (status == BookingStatus::FAILED) return "FAILED";
        return "PENDING";
    }
};

#endif
