#pragma once
#include <QVector>
#include "movie.h"

struct MovieResponse {
    bool success;
    QString errorMessage;
    QVector<QString> data;
};

struct GetMoviesResponse {
    bool success;
    QString errorMessage;
    QVector<Movie> movies;
};
