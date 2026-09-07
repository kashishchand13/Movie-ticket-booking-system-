#ifndef SHOW_CPP
#define SHOW_CPP

#include <string>
#include <vector>
#include <iostream>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"
using namespace std;

// Responsibility: one screening = a Movie on a Screen at a given start time.
// Owns (composition) the ShowSeats created for this screening; borrows (aggregation)
// the Movie and the Screen, neither of which is destroyed when the Show ends.
class Show {
private:
    string showId;
    const Movie* movie;
    const Screen* screen;
    string startTime;
    vector<ShowSeat> showSeats;

public:
    Show(string showId, const Movie* movie, const Screen* screen, string startTime)
        : showId(showId), movie(movie), screen(screen), startTime(startTime) {
        for (const Seat& seat : screen->getSeats()) {
            showSeats.push_back(ShowSeat(&seat));
        }
    }

    string getShowId() const { return showId; }
    const Movie* getMovie() const { return movie; }
    const Screen* getScreen() const { return screen; }
    string getStartTime() const { return startTime; }
    vector<ShowSeat>& getShowSeats() { return showSeats; }

    // Must NOT do: this only finds a seat, it never changes booking state.
    ShowSeat* findShowSeat(const string& seatNumber) {
        for (ShowSeat& showSeat : showSeats) {
            if (showSeat.getSeat()->getSeatNumber() == seatNumber) {
                return &showSeat;
            }
        }
        return nullptr;
    }

    // Printing only - a view of the Show's seats, not a separate class.
    void printLayout() const {
        cout << "\n  SCREEN-" << screen->getScreenNumber() << "  " << startTime
             << "  |  " << movie->getTitle() << "\n";
        printRow("SILVER", SeatType::SILVER);
        printRow("GOLD", SeatType::GOLD);
        printRow("PLATINUM", SeatType::PLATINUM);
        cout << "\n  ( [ ] = available   [X] = booked )\n";
    }

private:
    void printRow(const string& label, SeatType type) const {
        cout << "  " << label;
        for (int i = label.size(); i < 9; i++) cout << " ";
        for (const ShowSeat& showSeat : showSeats) {
            if (showSeat.getSeat()->getType() == type) {
                cout << showSeat.getSeat()->getSeatNumber()
                     << (showSeat.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        cout << "\n";
    }
};

#endif
