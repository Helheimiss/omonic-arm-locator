//
// Created by helh on 11.04.2026.
//
#pragma once


#include <sqlite3.h>
#include <functional>
#include <memory>
#include <string>

class SqlLite3Database {
public:
    explicit SqlLite3Database(std::string_view path) noexcept(false);
    ~SqlLite3Database();

    SqlLite3Database(const SqlLite3Database &other) = delete;
    SqlLite3Database & operator=(const SqlLite3Database &other) = delete;

    SqlLite3Database(SqlLite3Database &&other) noexcept;
    SqlLite3Database & operator=(SqlLite3Database &&other) noexcept;

    void exec(std::string_view query, std::function<void(sqlite3_stmt *)> callback_bind=nullptr) const noexcept(false);

private:
    sqlite3 *db;
};

inline std::shared_ptr<SqlLite3Database> db;