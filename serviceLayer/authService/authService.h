#pragma once

#include "authResponse.h"

namespace AuthService {
    AuthResponse signUp(const QString &username, const QString &password);

    AuthResponse signIn(const QString &username, const QString &password);

    AuthResponse logout();
}
