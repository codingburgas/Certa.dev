#include "authRepository.h"
#include <QSqlQuery>

namespace AuthRepository {
    SignUpStatus signUp(const QString &username, const QString &password) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);

        if (!query.exec()) {
            return SignUpStatus::Failure;
        }

        return SignUpStatus::Success;
    }

    SignInStatus signIn(const QString &username, const QString &password) {
        QSqlQuery query;
        query.prepare("SELECT password FROM users WHERE username = ?");
        query.addBindValue(username);

        if (!query.exec()) {
            return SignInStatus::Failure;
        }

        if (!query.next()) {
            return SignInStatus::UserNotFound;
        }

        QString storedPassword = query.value(0).toString();
        if (password != storedPassword) {
            return SignInStatus::InvalidPassword;
        }

        return SignInStatus::Success;
    }

    bool userExists(const QString &username) {
        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE username = ?");
        query.addBindValue(username);
        return query.exec() && query.next();
    }
}
