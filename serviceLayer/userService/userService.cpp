#include "userService.h"
#include "authRepository.h"
#include "passwordHasher.h"
#include "userRepository.h"

namespace UserService {
    UserResponse saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre) {
        switch (UserRepository::saveInterestsGenre(username, interestsGenre)) {
            case SaveInterestsGenreStatus::Success:
                return {.success = true, .errorMessage = ""};
            case SaveInterestsGenreStatus::UserNotFound:
                return {.success = false, .errorMessage = "User not found."};
            case SaveInterestsGenreStatus::GenreNotFound:
                return {.success = false, .errorMessage = "One or more genres are invalid."};
            case SaveInterestsGenreStatus::Failure:
                return {.success = false, .errorMessage = "Failed to save interests."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }

    UserResponse updateUsername(const QString &currentUsername, const QString &newUsername) {
        switch (UserRepository::updateUsername(currentUsername, newUsername)) {
            case UpdateUsernameStatus::Success:
                return {.success = true, .errorMessage = ""};
            case UpdateUsernameStatus::UserNotFound:
                return {.success = false, .errorMessage = "User not found."};
            case UpdateUsernameStatus::UsernameTaken:
                return {.success = false, .errorMessage = "That username is already taken."};
            case UpdateUsernameStatus::Failure:
                return {.success = false, .errorMessage = "Failed to update username."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }

    UserResponse changePassword(const QString &username, const QString &currentPassword, const QString &newPassword) {
        QString storedHashedPassword;
        switch (AuthRepository::signIn(username, storedHashedPassword)) {
            case SignInStatus::Success:
                if (!PasswordHasher::verifyPassword(currentPassword, storedHashedPassword)) {
                    return {.success = false, .errorMessage = "Current password is incorrect."};
                }
                break;
            case SignInStatus::UserNotFound:
                return {.success = false, .errorMessage = "User not found."};
            case SignInStatus::Failure:
                return {.success = false, .errorMessage = "Failed to verify current password."};
        }

        const QString hashedPassword = PasswordHasher::hashPassword(newPassword);
        switch (UserRepository::updatePasswordHash(username, hashedPassword)) {
            case UpdatePasswordStatus::Success:
                return {.success = true, .errorMessage = ""};
            case UpdatePasswordStatus::UserNotFound:
                return {.success = false, .errorMessage = "User not found."};
            case UpdatePasswordStatus::Failure:
                return {.success = false, .errorMessage = "Failed to update password."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }

    UserResponse deleteUser(const QString &username) {
        switch (UserRepository::deleteUser(username)) {
            case DeleteUserStatus::Success:
                return {.success = true, .errorMessage = ""};
            case DeleteUserStatus::UserNotFound:
                return {.success = false, .errorMessage = "User not found."};
            case DeleteUserStatus::Failure:
                return {.success = false, .errorMessage = "Failed to delete account."};
        }

        return {.success = false, .errorMessage = "An unknown error occurred."};
    }
}
