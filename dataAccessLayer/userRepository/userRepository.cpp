#include "userRepository.h"
#include <QSqlQuery>

namespace UserRepository {
    UserInterestsResponse getUserInterests(const QString &username) {
        QSqlQuery query;
        query.prepare(R"(
            SELECT g.name
            FROM user_interests ui
            INNER JOIN users u ON u.id = ui.user_id
            INNER JOIN genres g ON g.id = ui.genre_id
            WHERE u.username = ?
        )");
        query.addBindValue(username);

        if (!query.exec()) {
            return {.success = false, .errorMessage = "Failed to fetch user interests.", .data = {}};
        }

        QVector<QString> interests;
        while (query.next()) {
            QString interest = query.value(0).toString().trimmed();

            if (interest != "") {
                interests.append(interest);
            }
        }

        return {.success = true, .errorMessage = "", .data = interests};
    }

    SaveInterestsGenreStatus saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre) {
        QSqlQuery getUserId;
        getUserId.prepare("SELECT id FROM users WHERE username = ?");
        getUserId.addBindValue(username);
        if (!getUserId.exec() || !getUserId.next()) {
            return SaveInterestsGenreStatus::UserNotFound;
        }

        int userId = getUserId.value(0).toInt();

        QSqlQuery insert;
        insert.prepare("INSERT OR IGNORE INTO user_interests (user_id, genre_id) VALUES (?, ?)");
        bool hasInvalidGenre = false;

        for (int i = 0; i < interestsGenre.size(); i++) {
            const QString genre = interestsGenre[i];

            QSqlQuery getGenreId;
            getGenreId.prepare("SELECT id FROM genres WHERE name = ?");
            getGenreId.addBindValue(genre);
            if (!getGenreId.exec() || !getGenreId.next()) {
                hasInvalidGenre = true;
                continue;
            }

            int genreId = getGenreId.value(0).toInt();
            insert.addBindValue(userId);
            insert.addBindValue(genreId);

            if (!insert.exec()) {
                return SaveInterestsGenreStatus::Failure;
            }
        }

        if (hasInvalidGenre) {
            return SaveInterestsGenreStatus::GenreNotFound;
        }

        return SaveInterestsGenreStatus::Success;
    }
}
