//
// Created by helh on 11.04.2026.
//

#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace armDB {
class ArmDB {
public:
    ArmDB(QString type, QString HostName, QString DatabaseName, QString UserName, QString Password, int port = 3306);
    // ArmDB(QString type, QString HostName, QString DatabaseName, int port);
    // ArmDB(QString type, QString HostName, QString DatabaseName);
    ArmDB(QSqlDatabase db);

    ~ArmDB();

    QSqlQuery getQuery();

private:
    QSqlDatabase db;
};
} // armDB