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

    ArmDB(const ArmDB &other) = delete;
    ArmDB & operator=(const ArmDB &other) = delete;

    // TODO()
    // ArmDB(ArmDB &&other) noexcept;
    // ArmDB & operator=(ArmDB &&other) noexcept;

    QSqlQuery getQuery();
    void tryCreateTable();
    void tryInsertLogsToDB(QString UID, QString IP, QString HostName, QString SubDivision, QString Domain, QString Workgroup);

private:
    QString type;
    QSqlDatabase db;
};

inline std::unique_ptr<ArmDB> DB;
} // armDB