#pragma once

#include "userResponse.h"

namespace UserService {
    UserResponse saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre);
}
