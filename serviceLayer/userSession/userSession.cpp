#include "userSession.h"
#include "user.h"

UserSession &UserSession::instance() {
    static UserSession session;
    return session;
}

void UserSession::setCurrentUser(const User &user) {
    _currentUser = user;
}

std::optional<User> UserSession::getCurrentUser() {
    return _currentUser;
}

void UserSession::clearUser() {
    _currentUser.reset();
}

bool UserSession::isLoggedIn() {
    return _currentUser.has_value();
}

void UserSession::setInterestsGenres(const QVector<QString> &interestsGenres) {
    if (_currentUser.has_value()) {
        _currentUser->interestsGenres = interestsGenres;
    }
}


