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
            "SELECT m.id, m.title, m.description, m.year, m.director, m.rating, m.poster_path, "
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

        QVector<Movie> movies;
        while (query.next()) {
            Movie movie;
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
}
