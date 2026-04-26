#pragma once

enum class SignUpStatus {
    Success,
    Failure
};

enum class SignInStatus {
    Success,
    UserNotFound,
    InvalidPassword,
    Failure
};
