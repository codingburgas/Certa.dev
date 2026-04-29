#pragma once

#include "userResponse.h"

namespace UserService {
    UserResponse saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre);

    UserResponse updateUsername(const QString &currentUsername, const QString &newUsername);

    UserResponse changePassword(const QString &username, const QString &currentPassword, const QString &newPassword);

    UserResponse deleteUser(const QString &username);
}
