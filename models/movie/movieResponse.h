#pragma once
#include <QVector>
#include "movieDto.h"

struct MovieResponse {
    bool success;
    QString errorMessage;
    QVector<QString> data;
};

struct GetMoviesResponse {
    bool success;
    QString errorMessage;
    QVector<MovieDto> movies;
};

struct GetMovieResponse {
    bool success;
    QString errorMessage;
    MovieDto movie;
};

struct ReviewResponse {
    bool success;
    QString errorMessage;
};

struct GetReviewsResponse {
    bool success;
    QString errorMessage;
    QVector<Review> reviews;
};

struct FavoriteResponse {
    bool success;
    QString errorMessage;
};

struct FavoriteStatusResponse {
    bool success;
    QString errorMessage;
    bool isFavorite;
};
