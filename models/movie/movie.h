#pragma once
#include <QString>

struct Movie {
    int id;
    QString title;
    QString description;
    int year;
    QString director;
    double rating;
    QString posterPath;
    int reviewCount;
    QString genres;
};
