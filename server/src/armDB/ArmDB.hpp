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
    ArmDB(const QString &type, const QString &HostName, const QString &DatabaseName, const QString &UserName, const QString &Password, int port = 3306);
    ~ArmDB();

    ArmDB(const ArmDB &other) = delete;
    ArmDB & operator=(const ArmDB &other) = delete;

    ArmDB(ArmDB &&other) = delete;
    ArmDB & operator=(ArmDB &&other) = delete;

    QSqlQuery getQuery();
    void tryCreateTable();
    void tryInsertLogsToDB(QString UID, QString IP, QString HostName, QString SubDivision, QString Domain, QString Workgroup);

private:
    QString type;
    QSqlDatabase db;
    QString user_name_;
};

inline std::unique_ptr<ArmDB> DB;
} // armDB