#ifndef SEAT_CPP
#define SEAT_CPP

#include <string>
using namespace std;

// Encapsulation + constants instead of magic numbers.
enum class SeatType { SILVER, GOLD, PLATINUM };

const double SILVER_PRICE = 150.0;
const double GOLD_PRICE = 250.0;
const double PLATINUM_PRICE = 400.0;

string seatTypeToString(SeatType type) {
    if (type == SeatType::SILVER) return "SILVER";
    if (type == SeatType::GOLD) return "GOLD";
    return "PLATINUM";
}

double seatTypePrice(SeatType type) {
    if (type == SeatType::SILVER) return SILVER_PRICE;
    if (type == SeatType::GOLD) return GOLD_PRICE;
    return PLATINUM_PRICE;
}

// Responsibility: one physical seat - its number and type. Nothing about booking status.
class Seat {
private:
    string seatNumber;
    SeatType type;

public:
    Seat(string seatNumber, SeatType type) : seatNumber(seatNumber), type(type) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
};

#endif
