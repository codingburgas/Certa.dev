#pragma once
#include <QSqlDatabase>

class Database {
public:
    static Database &instance();

    bool connect(const QString &dbFileName);

    bool isConnected() const;

private:
    QSqlDatabase _db;
    bool _connected = false;
};
