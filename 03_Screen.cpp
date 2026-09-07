#ifndef SCREEN_CPP
#define SCREEN_CPP

#include <vector>
#include "02_Seat.cpp"
using namespace std;

// Responsibility: one auditorium - its screen number and the physical seats inside it.
// Composition with Seat: a Seat has no meaning outside its Screen.
class Screen {
private:
    int screenNumber;
    vector<Seat> seats;

public:
    Screen(int screenNumber) : screenNumber(screenNumber) {}

    void addSeat(const Seat& seat) {
        seats.push_back(seat);
    }

    int getScreenNumber() const { return screenNumber; }
    const vector<Seat>& getSeats() const { return seats; }
};

#endif
