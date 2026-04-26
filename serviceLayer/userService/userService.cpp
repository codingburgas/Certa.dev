#include "userService.h"
#include "userRepository.h"

namespace UserService {
    UserResponse saveInterestsGenre(const QString &username, const QVector<QString> &interestsGenre) {
        if (interestsGenre.isEmpty()) {
            return {.success = true, .errorMessage = ""};
        }

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
}
