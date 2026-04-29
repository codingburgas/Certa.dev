#pragma once

enum class SaveInterestsGenreStatus {
    Success,
    UserNotFound,
    GenreNotFound,
    Failure
};

enum class UpdateUsernameStatus {
    Success,
    UserNotFound,
    UsernameTaken,
    Failure
};

enum class UpdatePasswordStatus {
    Success,
    UserNotFound,
    Failure
};

enum class DeleteUserStatus {
    Success,
    UserNotFound,
    Failure
};

enum class GetUserInterestsStatus {
    Success,
    Failure
};
