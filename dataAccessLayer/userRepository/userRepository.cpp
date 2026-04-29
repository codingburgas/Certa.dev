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

        QSqlQuery clearExisting;
        clearExisting.prepare("DELETE FROM user_interests WHERE user_id = ?");
        clearExisting.addBindValue(userId);
        if (!clearExisting.exec()) {
            return SaveInterestsGenreStatus::Failure;
        }

        if (interestsGenre.isEmpty()) {
            return SaveInterestsGenreStatus::Success;
        }

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

    UpdateUsernameStatus updateUsername(const QString &currentUsername, const QString &newUsername) {
        if (currentUsername == newUsername) {
            return UpdateUsernameStatus::Success;
        }

        QSqlQuery getUserId;
        getUserId.prepare("SELECT id FROM users WHERE username = ?");
        getUserId.addBindValue(currentUsername);
        if (!getUserId.exec() || !getUserId.next()) {
            return UpdateUsernameStatus::UserNotFound;
        }

        QSqlQuery checkNew;
        checkNew.prepare("SELECT id FROM users WHERE username = ?");
        checkNew.addBindValue(newUsername);
        if (checkNew.exec() && checkNew.next()) {
            return UpdateUsernameStatus::UsernameTaken;
        }

        int userId = getUserId.value(0).toInt();

        QSqlQuery update;
        update.prepare("UPDATE users SET username = ? WHERE id = ?");
        update.addBindValue(newUsername);
        update.addBindValue(userId);

        if (!update.exec()) {
            return UpdateUsernameStatus::Failure;
        }

        return UpdateUsernameStatus::Success;
    }

    UpdatePasswordStatus updatePasswordHash(const QString &username, const QString &hashedPassword) {
        QSqlQuery getUserId;
        getUserId.prepare("SELECT id FROM users WHERE username = ?");
        getUserId.addBindValue(username);
        if (!getUserId.exec() || !getUserId.next()) {
            return UpdatePasswordStatus::UserNotFound;
        }

        QSqlQuery update;
        update.prepare("UPDATE users SET password = ? WHERE username = ?");
        update.addBindValue(hashedPassword);
        update.addBindValue(username);

        if (!update.exec()) {
            return UpdatePasswordStatus::Failure;
        }

        return UpdatePasswordStatus::Success;
    }

    DeleteUserStatus deleteUser(const QString &username) {
        QSqlQuery getUserId;
        getUserId.prepare("SELECT id FROM users WHERE username = ?");
        getUserId.addBindValue(username);
        if (!getUserId.exec() || !getUserId.next()) {
            return DeleteUserStatus::UserNotFound;
        }

        int userId = getUserId.value(0).toInt();

        QSqlQuery deleteInterests;
        deleteInterests.prepare("DELETE FROM user_interests WHERE user_id = ?");
        deleteInterests.addBindValue(userId);
        if (!deleteInterests.exec()) {
            return DeleteUserStatus::Failure;
        }

        QSqlQuery deleteUser;
        deleteUser.prepare("DELETE FROM users WHERE id = ?");
        deleteUser.addBindValue(userId);
        if (!deleteUser.exec()) {
            return DeleteUserStatus::Failure;
        }

        return DeleteUserStatus::Success;
    }
}
