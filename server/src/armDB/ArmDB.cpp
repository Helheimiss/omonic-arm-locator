//
// Created by helh on 11.04.2026.
//

#include "ArmDB.hpp"

using namespace std::string_literals;

namespace armDB {
ArmDB::ArmDB(const QString &type, const QString &HostName, const QString &DatabaseName, const QString &UserName, const QString &Password, int port) : type(type) {
    db = QSqlDatabase::addDatabase(type);

    db.setHostName(HostName);
    db.setDatabaseName(DatabaseName);
    db.setUserName(UserName);
    db.setPassword(Password);
    db.setPort(port);

    if (!db.open()) {
        throw std::runtime_error("Could not connect to database "s + db.lastError().text().toStdString());
    }

}

ArmDB::~ArmDB() {
    db.close();
}

QSqlQuery ArmDB::getQuery() {
    return QSqlQuery(db);
}

void ArmDB::tryCreateTable() {
    // TODO()
    auto query = getQuery();
    QString sql;

    if (type == "QMARIADB") {
        sql = R"(
        CREATE TABLE IF NOT EXISTS `arm-locator-logs` (
          `UID` varchar(255) PRIMARY KEY,
          `DatePing` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
          `IP` text NOT NULL,
          `HostName` text NOT NULL,
          `SubDivision` text NOT NULL,
          `Domain` text NOT NULL,
          `WorkGroup` text NOT NULL
        ))";
    }
    else {
        throw std::runtime_error("bad type DB");
    }

    if (!query.prepare(sql)) {
        throw std::runtime_error("Prepare failed: " + query.lastError().text().toStdString());
    }

    if (!query.exec()) {
        throw std::runtime_error("Exec failed: " + query.lastError().text().toStdString());
    }
}

void ArmDB::tryInsertLogsToDB(const QString &UID, const QString &IP, const QString &HostName, const QString &SubDivision, const QString &Domain, const QString &Workgroup) {
    // TODO()
    QString sql = R"(
    INSERT INTO `arm-locator-logs` (UID, IP, HostName, Subdivision, Domain, Workgroup)
    VALUES (?, ?, ?, ?, ?, ?)
    ON DUPLICATE KEY UPDATE
        IP = VALUES(IP),
        DatePing = CURRENT_TIMESTAMP(),
        HostName = VALUES(HostName),
        Subdivision = VALUES(Subdivision),
        Domain = VALUES(Domain),
        Workgroup = VALUES(Workgroup);
    )";

    auto query = getQuery();

    if (!query.prepare(sql)) {
        throw std::runtime_error("Prepare failed: " + query.lastError().text().toStdString());
    }

    query.bindValue(0, UID);
    query.bindValue(1, IP);
    query.bindValue(2, HostName);
    query.bindValue(3, SubDivision);
    query.bindValue(4, Domain);
    query.bindValue(5, Workgroup);

    if (!query.exec()) {
        throw std::runtime_error("Exec failed: " + query.lastError().text().toStdString());
    }
}
} // armDB