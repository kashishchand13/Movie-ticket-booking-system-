#include <iostream>
#include <sstream>
#include <vector>
#include <limits>
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "13_BookingService.cpp"
using namespace std;

// ---- setup helpers (kept out of main() so main() stays short and does ONE thing) ----

Screen buildScreen(int screenNumber) {
    Screen screen(screenNumber);
    screen.addSeat(Seat("A1", SeatType::SILVER));
    screen.addSeat(Seat("A2", SeatType::SILVER));
    screen.addSeat(Seat("A3", SeatType::SILVER));
    screen.addSeat(Seat("A4", SeatType::SILVER));
    screen.addSeat(Seat("B1", SeatType::GOLD));
    screen.addSeat(Seat("B2", SeatType::GOLD));
    screen.addSeat(Seat("B3", SeatType::GOLD));
    screen.addSeat(Seat("C1", SeatType::PLATINUM));
    screen.addSeat(Seat("C2", SeatType::PLATINUM));
    return screen;
}

vector<string> splitBySeatSeparator(const string& text) {
    vector<string> seatNumbers;
    stringstream stream(text);
    string seatNumber;
    while (getline(stream, seatNumber, ',')) {
        if (!seatNumber.empty()) seatNumbers.push_back(seatNumber);
    }
    return seatNumbers;
}

int readMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid choice, enter a number: ";
    }
    return choice;
}

void printMovieList(const vector<Movie>& movies) {
    for (size_t i = 0; i < movies.size(); i++) {
        cout << "  [" << (i + 1) << "] " << movies[i].getTitle()
             << "\t" << movies[i].getLanguage()
             << "\t" << movies[i].getDurationMinutes() << " min\n";
    }
}

// FR2: shows for one chosen movie.
void printShowList(const vector<Show*>& shows, const Movie* movie) {
    int shownIndex = 1;
    for (Show* show : shows) {
        if (show->getMovie() == movie) {
            cout << "  [" << shownIndex << "] Screen-" << show->getScreen()->getScreenNumber()
                 << "\t" << show->getStartTime() << "\n";
            shownIndex++;
        }
    }
}

vector<Show*> showsForMovie(const vector<Show*>& shows, const Movie* movie) {
    vector<Show*> result;
    for (Show* show : shows) {
        if (show->getMovie() == movie) result.push_back(show);
    }
    return result;
}

Payment* collectPayment(double amount) {
    cout << "\n  Pay by: 1.UPI  2.Card  3.Cash > ";
    int methodChoice = readMenuChoice();
    if (methodChoice == 1) {
        cout << "  Enter UPI id: ";
        string upiId; cin >> upiId;
        return new UpiPayment(amount, upiId);
    }
    if (methodChoice == 2) {
        cout << "  Enter 16-digit card number: ";
        string cardNumber; cin >> cardNumber;
        return new CardPayment(amount, cardNumber);
    }
    cout << "  Enter cash tendered: ";
    double tendered; cin >> tendered;
    return new CashPayment(amount, tendered);
}

// FR1 + F2 + F3 + F4/F5/F6/F7 tied together: the "Book" menu option.
void handleBook(vector<Movie>& movies, vector<Show*>& shows, BookingService& bookingService,
                Customer& customer) {
    cout << "\n";
    printMovieList(movies);
    cout << "Choose movie: ";
    int movieChoice = readMenuChoice();
    bool isValidMovie = movieChoice >= 1 && movieChoice <= (int)movies.size();
    if (!isValidMovie) { cout << "  Invalid movie choice.\n"; return; }
    Movie* movie = &movies[movieChoice - 1];

    vector<Show*> movieShows = showsForMovie(shows, movie);
    printShowList(movieShows, movie);
    cout << "Choose show: ";
    int showChoice = readMenuChoice();
    bool isValidShow = showChoice >= 1 && showChoice <= (int)movieShows.size();
    if (!isValidShow) { cout << "  Invalid show choice.\n"; return; }
    Show* show = movieShows[showChoice - 1];

    show->printLayout();
    cout << "\nSeats (e.g. A1,B2): ";
    string seatLine; cin >> seatLine;
    vector<string> seatNumbers = splitBySeatSeparator(seatLine);
    if (seatNumbers.empty()) { cout << "  No seats entered.\n"; return; }

    // F4/FR4: reject whole booking if any seat is invalid/already booked.
    Booking* booking = bookingService.createBooking(show, seatNumbers, customer);
    if (booking == nullptr) return;

    for (ShowSeat* seat : booking->getSeats()) {
        cout << "  " << seat->getSeat()->getSeatNumber() << " "
             << seatTypeToString(seat->getSeat()->getType()) << " Rs."
             << seatTypePrice(seat->getSeat()->getType()) << "\n";
    }
    cout << "  TOTAL          Rs." << booking->getTotalAmount() << "\n";

    // F6/FR6: payment; failure must not confirm the booking.
    Payment* payment = collectPayment(booking->getTotalAmount());
    bool paid = bookingService.makePayment(booking, payment);
    if (!paid) {
        cout << "  Booking " << booking->getBookingId() << " NOT confirmed. Seats remain AVAILABLE.\n";
        return;
    }

    // F7/FR7: print the ticket only after a CONFIRMED booking.
    bookingService.printTicket(*booking);
}

void handleCancel(BookingService& bookingService) {
    cout << "\nEnter booking id to cancel: ";
    string bookingId; cin >> bookingId;
    bookingService.cancelBooking(bookingId);
}

void handleMyTickets(BookingService& bookingService, TicketPrinter& printer) {
    const vector<Booking*>& bookings = bookingService.getAllBookings();
    if (bookings.empty()) { cout << "\n  No bookings yet.\n"; return; }
    for (Booking* booking : bookings) {
        printer.printTicket(*booking);
    }
}

int main() {
    Cinema cinema("PVR");
    Screen screen1 = buildScreen(1);
    Screen screen2 = buildScreen(2);
    cinema.addScreen(screen1);
    cinema.addScreen(screen2);

    vector<Movie> movies;
    movies.push_back(Movie("M1", "3 Idiots", "Hindi", 170));
    movies.push_back(Movie("M2", "Interstellar", "English", 169));

    vector<Show*> shows;
    shows.push_back(new Show("S1", &movies[0], &cinema.getScreens()[0], "06:00 PM"));
    shows.push_back(new Show("S2", &movies[0], &cinema.getScreens()[1], "09:00 PM"));
    shows.push_back(new Show("S3", &movies[1], &cinema.getScreens()[0], "09:30 PM"));

    // Pre-existing bookings so the seat layout matches a real cinema (some seats
    // already BOOKED before this session starts) - demonstrates FR3/F3 directly.
    shows[0]->findShowSeat("A2")->book();
    shows[0]->findShowSeat("B3")->book();

    BookingService bookingService;
    TicketPrinter ticketPrinter;

    cout << "Customer name: ";
    string customerName; cin >> customerName;
    Customer customer(customerName);   // overloaded constructor - no phone supplied

    cout << "\n===== MOVIE TICKET BOOKING =====\n";
    bool running = true;
    while (running) {
        cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
        int choice = readMenuChoice();
        switch (choice) {
            case 1: cout << "\n"; printMovieList(movies); break;
            case 2: handleBook(movies, shows, bookingService, customer); break;
            case 3: handleCancel(bookingService); break;
            case 4: handleMyTickets(bookingService, ticketPrinter); break;
            case 0: running = false; break;
            default: cout << "  Invalid menu choice.\n";
        }
    }

    for (Show* show : shows) delete show;
    cout << "Goodbye!\n";
    return 0;
}
