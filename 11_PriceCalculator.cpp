#ifndef PRICECALCULATOR_CPP
#define PRICECALCULATOR_CPP

#include <vector>
#include "06_ShowSeat.cpp"
using namespace std;

// Responsibility: turn a list of seats into a total amount. Must NOT book,
// print, or know about Payment/Booking at all - a pricing change (e.g. a
// weekend surcharge) should only ever touch this one class.
class PriceCalculator {
public:
    double calculateTotal(const vector<ShowSeat*>& seats) const {
        double total = 0.0;
        for (ShowSeat* seat : seats) {
            total += seatTypePrice(seat->getSeat()->getType());
        }
        return total;
    }
};

#endif
