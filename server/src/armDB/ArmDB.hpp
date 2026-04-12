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
    ArmDB(const QString &type, const QString &HostName, const QString &DatabaseName, const QString &UserName, const QString &Password, const QString &connectOptions = QString(), int port = 3306);
    ~ArmDB();

    ArmDB(const ArmDB &other) = delete;
    ArmDB & operator=(const ArmDB &other) = delete;

    ArmDB(ArmDB &&other) = delete;
    ArmDB & operator=(ArmDB &&other) = delete;

    QSqlQuery getQuery();
    void tryCreateTable();
    void tryInsertLogsToDB(const QString &UID, const QString &IP, const QString &HostName, const QString &SubDivision, const QString &Domain, const QString &Workgroup);

private:
    QString type;
    QSqlDatabase db;
    QString user_name_;
};

inline std::unique_ptr<ArmDB> DB;
} // armDB