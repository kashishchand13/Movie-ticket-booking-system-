#ifndef MOVIE_CPP
#define MOVIE_CPP

#include <string>
using namespace std;

// Responsibility: hold a movie's title, language and duration. Nothing else.
class Movie {
private:
    string movieId;
    string title;
    string language;
    int durationMinutes;

public:
    Movie(string movieId, string title, string language, int durationMinutes)
        : movieId(movieId), title(title), language(language), durationMinutes(durationMinutes) {}

    string getMovieId() const { return movieId; }
    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDurationMinutes() const { return durationMinutes; }
};

#endif
