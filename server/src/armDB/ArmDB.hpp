//
// Created by helh on 11.04.2026.
//

#pragma once

#include <nanodbc.h>
#include <memory>
#include <string>

namespace armDB {
class ArmDB {
public:
    ArmDB(std::string env, std::string table_name);
    ~ArmDB();

    ArmDB(const ArmDB &other) = delete;
    ArmDB & operator=(const ArmDB &other) = delete;

    ArmDB(ArmDB &&other) = delete;
    ArmDB & operator=(ArmDB &&other) = delete;

    nanodbc::result execute(const std::string &sql);
    static nanodbc::result execute(nanodbc::statement &stmt);
    nanodbc::statement getStatement();

    void tryCreateTable();
    void tryInsertLogsToDB(const std::string &UID, const std::string &IP, const std::string &HostName, const std::string &SubDivision, const std::string &Domain, const std::string &Workgroup);

private:
    nanodbc::connection db;
    std::string table_name;
};

inline std::unique_ptr<ArmDB> DB;
} // armDB