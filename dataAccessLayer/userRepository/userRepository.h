#pragma once
#include "userResponse.h"
#include "userStatus.h"

namespace UserRepository {
    UserInterestsResponse getUserInterests(const QString &username);

    SaveInterestsGenreStatus saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre);

    UpdateUsernameStatus updateUsername(const QString &currentUsername, const QString &newUsername);

    UpdatePasswordStatus updatePasswordHash(const QString &username, const QString &hashedPassword);

    DeleteUserStatus deleteUser(const QString &username);
}
