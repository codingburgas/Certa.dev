#pragma once

#include "movieResponse.h"

namespace MovieService {
    MovieResponse getAllGenres();

    GetMoviesResponse getAllMovies();

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit = -1);

    GetMovieResponse getMovieById(int id);

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment);

    GetReviewsResponse getReviewsForMovie(int movieId);
}
