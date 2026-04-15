//
// Created by helh on 11.04.2026.
//

#include "ArmDB.hpp"

using namespace std::string_literals;

namespace armDB {
ArmDB::ArmDB(std::string env) {
    db.connect(env);

    if (!db.connected())
        throw std::runtime_error("DB connection failed");
}

ArmDB::~ArmDB() {
    db.disconnect();
}


nanodbc::result ArmDB::execute(const std::string &sql) {
    return nanodbc::execute(db, sql);
}

nanodbc::result ArmDB::execute(nanodbc::statement &stmt) {
    return nanodbc::execute(stmt);
}

nanodbc::statement ArmDB::getStatement() {
    return nanodbc::statement(db);
}

void ArmDB::tryCreateTable() {
    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS `omn_arm_locator_logs` (
          `UID` varchar(255) PRIMARY KEY,
          `DatePing` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
          `IP` text NOT NULL,
          `HostName` text NOT NULL,
          `SubDivision` text NOT NULL,
          `Domain` text NOT NULL,
          `WorkGroup` text NOT NULL
        ))";

    try {
        execute(sql);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }

}

void ArmDB::tryInsertLogsToDB(const std::string &UID, const std::string &IP, const std::string &HostName, const std::string &SubDivision, const std::string &Domain, const std::string &Workgroup) {
    std::string sql = R"(
    INSERT INTO `omn_arm_locator_logs` (UID, IP, HostName, Subdivision, Domain, Workgroup)
    VALUES (?, ?, ?, ?, ?, ?)
    ON DUPLICATE KEY UPDATE
        IP = VALUES(IP),
        DatePing = CURRENT_TIMESTAMP(),
        HostName = VALUES(HostName),
        Subdivision = VALUES(Subdivision),
        Domain = VALUES(Domain),
        Workgroup = VALUES(Workgroup);
    )";

    try {
        auto stmt = getStatement();

        stmt.bind(0, UID.c_str());
        stmt.bind(1, IP.c_str());
        stmt.bind(2, HostName.c_str());
        stmt.bind(3, SubDivision.c_str());
        stmt.bind(4, Domain.c_str());
        stmt.bind(5, Workgroup.c_str());

        nanodbc::execute(stmt);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Database error: " + std::string(e.what()));
    }
}
} // armDB