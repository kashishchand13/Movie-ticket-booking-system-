#ifndef CINEMA_CPP
#define CINEMA_CPP

#include <string>
#include <vector>
#include "03_Screen.cpp"
using namespace std;

// Responsibility: the theatre - its name and the screens it owns. Nothing about shows or bookings.
class Cinema {
private:
    string name;
    vector<Screen> screens;

public:
    Cinema(string name) : name(name) {}

    void addScreen(const Screen& screen) {
        screens.push_back(screen);
    }

    string getName() const { return name; }
    vector<Screen>& getScreens() { return screens; }
};

#endif
