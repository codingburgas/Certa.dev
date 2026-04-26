#include "authService.h"
#include "authRepository.h"
#include "userRepository.h"
#include "user.h"
#include "userResponse.h"
#include "userSession.h"

namespace AuthService {
    AuthResponse signUp(const QString &username, const QString &password) {
        if (AuthRepository::userExists(username)) {
            return {.success = false, .errorMessage = "User already exists."};
        }

        switch (AuthRepository::signUp(username, password)) {
            case SignUpStatus::Success: {
                User user;
                user.username = username;
                user.interestsGenres = {};

                UserSession::instance().setCurrentUser(user);

                return {.success = true, .errorMessage = ""};
            }
            case SignUpStatus::Failure: return {.success = false, .errorMessage = "An error occurred during sign-up."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }

    AuthResponse signIn(const QString &username, const QString &password) {
        switch (AuthRepository::signIn(username, password)) {
            case SignInStatus::Success: {
                User user;
                user.username = username;

                UserInterestsResponse interestsResponse = UserRepository::getUserInterests(username);
                if (!interestsResponse.success) {
                    return {.success = false, .errorMessage = "An error occurred during sign-in."};
                }

                user.interestsGenres = interestsResponse.data;

                UserSession::instance().setCurrentUser(user);

                return {.success = true, .errorMessage = ""};
            }
            case SignInStatus::UserNotFound: return {.success = false, .errorMessage = "User not found."};
            case SignInStatus::InvalidPassword: return {.success = false, .errorMessage = "Invalid password."};
            case SignInStatus::Failure: return {.success = false, .errorMessage = "An error occurred during sign-in."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }

    AuthResponse logout() {
        if (!UserSession::instance().isLoggedIn()) {
            return {.success = false, .errorMessage = "No user is currently logged in."};
        }

        UserSession::instance().clearUser();

        return {.success = true, .errorMessage = ""};
    }
}
