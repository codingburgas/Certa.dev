#pragma once
#include <optional>
#include "user.h"

class UserSession {
public:
    static UserSession &instance();

    void setCurrentUser(const User &user);

    std::optional<User> getCurrentUser();

    void clearUser();

    bool isLoggedIn();

    void setInterestsGenres(const QVector<QString> &interestsGenres);

private:
    std::optional<User> _currentUser;
};
