#include "movieService.h"
#include "movieRepository.h"

namespace MovieService {
    MovieResponse getAllGenres() {
        return MovieRepository::getAllGenres();
    }

    GetMoviesResponse getAllMovies() {
        return MovieRepository::getAllMovies();
    }

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit) {
        return MovieRepository::getRecommendedMovies(username, limit);
    }

    GetMovieResponse getMovieById(int id) {
        return MovieRepository::getMovieById(id);
    }

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment) {
        return MovieRepository::createReview(username, movieId, rating, comment);
    }

    GetReviewsResponse getReviewsForMovie(int movieId) {
        return MovieRepository::getReviewsForMovie(movieId);
    }

    FavoriteStatusResponse isFavorite(const QString &username, int movieId) {
        return MovieRepository::isFavorite(username, movieId);
    }

    FavoriteResponse addFavorite(const QString &username, int movieId) {
        return MovieRepository::addFavorite(username, movieId);
    }

    FavoriteResponse removeFavorite(const QString &username, int movieId) {
        return MovieRepository::removeFavorite(username, movieId);
    }

    GetMoviesResponse getFavoriteMovies(const QString &username) {
        return MovieRepository::getFavoriteMovies(username);
    }
}
