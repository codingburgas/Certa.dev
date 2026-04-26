#pragma once
#include "userResponse.h"
#include "userStatus.h"

namespace UserRepository {
    UserInterestsResponse getUserInterests(const QString &username);

    SaveInterestsGenreStatus saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre);
}
