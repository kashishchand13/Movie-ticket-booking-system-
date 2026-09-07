#ifndef SHOWSEAT_CPP
#define SHOWSEAT_CPP

#include "02_Seat.cpp"

enum class SeatStatus { AVAILABLE, BOOKED };

// Responsibility: the status (AVAILABLE/BOOKED) of ONE physical seat, FOR ONE show.
// Why not just Seat? Seat A1 exists once in the screen, but its status differs per show.
class ShowSeat {
private:
    const Seat* seat;      // aggregation: refers to a Seat owned by the Screen
    SeatStatus status;

public:
    ShowSeat(const Seat* seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    bool isAvailable() const { return status == SeatStatus::AVAILABLE; }

    // Encapsulation: status can only change through these two controlled methods.
    void book() { status = SeatStatus::BOOKED; }
    void release() { status = SeatStatus::AVAILABLE; }

    const Seat* getSeat() const { return seat; }
    SeatStatus getStatus() const { return status; }
};

#endif
