#include "database.h"
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

Database &Database::instance() {
    static Database instance;
    return instance;
}

bool Database::connect(const QString &dbFileName) {
    if (_connected) return true;

    _db = QSqlDatabase::addDatabase("QSQLITE");

    // find assets folder from current (dataAccessLayer/database)
    QFileInfo sourceInfo(__FILE__);
    QDir dir = sourceInfo.absoluteDir();
    dir.cdUp();
    dir.cdUp();
    QString databasePath = dir.absoluteFilePath("assets/" + dbFileName);

    QFileInfo dbInfo(databasePath);

    _db.setDatabaseName(databasePath);

    _connected = dbInfo.exists() && _db.open() && _db.isOpen();

    if (_connected) {
        QSqlQuery pragma(_db);
        if (!pragma.exec("PRAGMA foreign_keys = ON")) {
            qDebug() << "Failed to enable foreign keys:" << pragma.lastError().text();
            _connected = false;
            return _connected;
        }
        qDebug() << "Using db at:" << databasePath;
    } else {
        qDebug() << "Could not open db at:" << databasePath;
    }

    return _connected;
}

bool Database::isConnected() const {
    return _connected;
}
