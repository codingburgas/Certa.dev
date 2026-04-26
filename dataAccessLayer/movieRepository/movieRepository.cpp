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
}
