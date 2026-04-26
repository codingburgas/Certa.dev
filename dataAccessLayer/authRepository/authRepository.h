#pragma once
#include <QString>
#include "authStatus.h"

namespace AuthRepository {
    SignUpStatus signUp(const QString &username, const QString &password);

    SignInStatus signIn(const QString &username, const QString &password);

    bool userExists(const QString &username);
}
