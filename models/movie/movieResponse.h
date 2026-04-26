#pragma once
#include <QString>
#include <QVector>

struct MovieResponse {
    bool success;
    QString errorMessage;
    QVector<QString> data;
};
