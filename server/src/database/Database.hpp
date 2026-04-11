//
// Created by helh on 11.04.2026.
//

#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


class Database {
public:
    Database(QString host, QString database, QString user, QString password);
    ~Database();

    void tryCreateDB();
    void tryInsertLogsToDB(QString UID, QString IP, QString HostName, QString SubDivision, QString Domain, QString Workgroup);

    QSqlQuery query();

private:
    QSqlDatabase db;
};