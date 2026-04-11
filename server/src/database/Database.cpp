//
// Created by helh on 11.04.2026.
//

#include "Database.hpp"

Database::Database(QString host, QString database, QString user, QString password) {
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(host);
    db.setDatabaseName(database);
    db.setUserName(user);
    db.setPassword(password);

    if (!db.open()) {
        throw std::runtime_error(db.lastError().text().toStdString());
    }

}

Database::~Database() {
    db.close();
}

void Database::tryCreateDB() {
    auto q = query();
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS `arm-locator-logs` (
            `UID` VARCHAR(255) PRIMARY KEY,
            `DatePing` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
            `IP` TEXT NOT NULL,
            `HostName` TEXT NOT NULL,
            `SubDivision` TEXT NOT NULL,
            `Domain` TEXT NOT NULL,
            `WorkGroup` TEXT NOT NULL
        ))";


    if (!q.exec(createTableSQL)) {
        throw std::runtime_error(db.lastError().text().toStdString());
    }
}

void Database::tryInsertLogsToDB(QString UID, QString IP, QString HostName, QString SubDivision, QString Domain, QString Workgroup) {
    QString sql = R"(
        INSERT INTO `arm-locator-logs` (UID, IP, HostName, Subdivision, Domain, Workgroup)
        VALUES (?, ?, ?, ?, ?, ?)
        ON DUPLICATE KEY UPDATE
            IP = VALUES(IP),
            HostName = VALUES(HostName),
            Subdivision = VALUES(Subdivision),
            Domain = VALUES(Domain),
            Workgroup = VALUES(Workgroup)
        )";

    auto q = query();
    q.prepare(sql);

    q.bindValue(0, UID);
    q.bindValue(1, IP);
    q.bindValue(2, HostName);
    q.bindValue(3, SubDivision);
    q.bindValue(4, Domain);
    q.bindValue(5, Workgroup);

    if (!q.exec()) {
        throw std::runtime_error(db.lastError().text().toStdString());
    }
}

QSqlQuery Database::query() {
    return QSqlQuery(db);
}
