#include "movieRepository.h"
#include <QSqlQuery>

namespace MovieRepository {
    MovieResponse getAllGenres() {
        QSqlQuery query;
        query.prepare("SELECT name FROM genres");

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch genres.", .data = {}};
        }

        QVector<QString> genres;
        while (query.next()) {
            QString genre = query.value(0).toString().trimmed();

            if (genre != "") {
                genres.append(genre);
            }
        }

        return {.success = true, .errorMessage = "", .data = genres};
    }

    GetMoviesResponse getAllMovies() {
        QSqlQuery query;
        query.prepare(
            "SELECT m.id, m.title, m.description, m.year, m.director, "
            "COALESCE(AVG(r.rating), 0) AS avg_rating, m.poster_path, "
            "COUNT(r.id) AS review_count, "
            "(SELECT GROUP_CONCAT(g.name, ', ') "
            " FROM movie_genres mg JOIN genres g ON g.id = mg.genre_id "
            " WHERE mg.movie_id = m.id) AS genres "
            "FROM movies m "
            "LEFT JOIN reviews r ON r.movie_id = m.id "
            "GROUP BY m.id "
            "ORDER BY m.title"
        );

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch movies.", .movies = {}};
        }

        QVector<MovieDto> movies;
        while (query.next()) {
            MovieDto movie;
            movie.id = query.value(0).toInt();
            movie.title = query.value(1).toString();
            movie.description = query.value(2).toString();
            movie.year = query.value(3).toInt();
            movie.director = query.value(4).toString();
            movie.rating = query.value(5).toDouble();
            movie.posterPath = query.value(6).toString();
            movie.reviewCount = query.value(7).toInt();
            movie.genres = query.value(8).toString();
            movies.append(movie);
        }

        return {.success = true, .errorMessage = "", .movies = movies};
    }

    GetMoviesResponse getRecommendedMovies(const QString &username, int limit) {
        QString sql =
            "SELECT m.id, m.title, m.description, m.year, m.director, "
            "COALESCE(AVG(r.rating), 0) AS avg_rating, m.poster_path, "
            "COUNT(r.id) AS review_count, "
            "(SELECT GROUP_CONCAT(g.name, ', ') "
            " FROM movie_genres mg JOIN genres g ON g.id = mg.genre_id "
            " WHERE mg.movie_id = m.id) AS genres, "
            "COUNT(DISTINCT mg2.genre_id) AS match_count "
            "FROM movies m "
            "JOIN movie_genres mg2 ON mg2.movie_id = m.id "
            "JOIN user_interests ui ON ui.genre_id = mg2.genre_id "
            "JOIN users u ON u.id = ui.user_id "
            "LEFT JOIN reviews r ON r.movie_id = m.id "
            "WHERE u.username = ? "
            "GROUP BY m.id "
            "ORDER BY match_count DESC, avg_rating DESC, m.title";

        if (limit > 0) {
            sql += " LIMIT ?";
        }

        QSqlQuery query;
        query.prepare(sql);
        query.addBindValue(username);
        if (limit > 0) {
            query.addBindValue(limit);
        }

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch recommended movies.", .movies = {}};
        }

        QVector<MovieDto> movies;
        while (query.next()) {
            MovieDto movie;
            movie.id = query.value(0).toInt();
            movie.title = query.value(1).toString();
            movie.description = query.value(2).toString();
            movie.year = query.value(3).toInt();
            movie.director = query.value(4).toString();
            movie.rating = query.value(5).toDouble();
            movie.posterPath = query.value(6).toString();
            movie.reviewCount = query.value(7).toInt();
            movie.genres = query.value(8).toString();
            movies.append(movie);
        }

        return {.success = true, .errorMessage = "", .movies = movies};
    }

    GetMovieResponse getMovieById(int id) {
        QSqlQuery query;
        query.prepare(
            "SELECT m.id, m.title, m.description, m.year, m.director, "
            "COALESCE(AVG(r.rating), 0) AS avg_rating, m.poster_path, "
            "COUNT(r.id) AS review_count, "
            "(SELECT GROUP_CONCAT(g.name, ', ') "
            " FROM movie_genres mg JOIN genres g ON g.id = mg.genre_id "
            " WHERE mg.movie_id = m.id) AS genres "
            "FROM movies m "
            "LEFT JOIN reviews r ON r.movie_id = m.id "
            "WHERE m.id = ? "
            "GROUP BY m.id"
        );
        query.addBindValue(id);

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch movie.", .movie = {}};
        }

        if (!query.next()) {
            return {.success = false, .errorMessage = "Movie not found.", .movie = {}};
        }

        MovieDto movie;
        movie.id = query.value(0).toInt();
        movie.title = query.value(1).toString();
        movie.description = query.value(2).toString();
        movie.year = query.value(3).toInt();
        movie.director = query.value(4).toString();
        movie.rating = query.value(5).toDouble();
        movie.posterPath = query.value(6).toString();
        movie.reviewCount = query.value(7).toInt();
        movie.genres = query.value(8).toString();

        return {.success = true, .errorMessage = "", .movie = movie};
    }

    ReviewResponse createReview(const QString &username, const int &movieId, const double &rating,
                                const QString &comment) {
        QSqlQuery getUserId;
        getUserId.prepare("SELECT id FROM users WHERE username = ?");
        getUserId.addBindValue(username);
        if (!getUserId.exec() || !getUserId.next()) {
            return {.success = false, .errorMessage = "User not found."};
        }

        int userId = getUserId.value(0).toInt();

        QSqlQuery existing;
        existing.prepare("SELECT 1 FROM reviews WHERE user_id = ? AND movie_id = ?");
        existing.addBindValue(userId);
        existing.addBindValue(movieId);
        if (!existing.exec()) {
            return {.success = false, .errorMessage = "Failed to check existing review."};
        }
        if (existing.next()) {
            return {.success = false, .errorMessage = "You have already reviewed this movie."};
        }

        QSqlQuery insert;
        insert.prepare("INSERT INTO reviews (user_id, movie_id, rating, comment) VALUES (?, ?, ?, ?)");
        insert.addBindValue(userId);
        insert.addBindValue(movieId);
        insert.addBindValue(rating);
        insert.addBindValue(comment);

        if (!insert.exec()) {
            return {.success = false, .errorMessage = "Failed to save review."};
        }

        return {.success = true, .errorMessage = ""};
    }

    GetReviewsResponse getReviewsForMovie(int movieId) {
        QSqlQuery query;
        query.prepare(
            "SELECT r.id, u.username, r.rating, r.comment "
            "FROM reviews r JOIN users u ON u.id = r.user_id "
            "WHERE r.movie_id = ? "
            "ORDER BY r.id DESC"
        );
        query.addBindValue(movieId);

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch reviews.", .reviews = {}};
        }

        QVector<Review> reviews;
        while (query.next()) {
            Review review;
            review.id = query.value(0).toInt();
            review.username = query.value(1).toString();
            review.rating = query.value(2).toDouble();
            review.comment = query.value(3).toString();
            reviews.append(review);
        }

        return {.success = true, .errorMessage = "", .reviews = reviews};
    }
}
