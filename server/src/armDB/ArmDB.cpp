//
// Created by helh on 11.04.2026.
//

#include "ArmDB.hpp"

using namespace std::string_literals;

namespace armDB {
ArmDB::ArmDB(QString type, QString HostName, QString DatabaseName, QString UserName, QString Password, int port) {
    db = QSqlDatabase::addDatabase(type, "my_connection");

    db.setHostName(HostName);
    db.setDatabaseName(DatabaseName);
    db.setUserName(UserName);
    db.setPassword(Password);
    db.setPort(port);

    if (!db.open()) {
        throw std::runtime_error("Could not connect to database "s + db.lastError().text().toStdString());
    }

}

ArmDB::ArmDB(QSqlDatabase db) {
    this->db = db;

    if (!this->db.open()) {
        throw std::runtime_error("Could not connect to database "s + db.lastError().text().toStdString());
    }
}

ArmDB::~ArmDB() {
    db.close();
}

QSqlQuery ArmDB::getQuery() {
    return QSqlQuery(db);
}
} // armDB