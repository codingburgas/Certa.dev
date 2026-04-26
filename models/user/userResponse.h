#pragma once

#include <QVector>

struct UserResponse {
    bool success;
    QString errorMessage;
};

struct UserInterestsResponse {
    bool success;
    QString errorMessage;
    QVector<QString> data;
};
