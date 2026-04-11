//
// Created by helh on 11.04.2026.
//

#include "Database.hpp"

#include <stdexcept>

Database::Database(std::string_view path) noexcept(false) : db(nullptr) {
    if (sqlite3_open(path.data(), &db) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));

    const char *sql = R"(
CREATE TABLE IF NOT EXISTS device_logs (
    UID TEXT PRIMARY KEY,
    Date INTEGER NOT NULL,
    IP TEXT,
    HostName TEXT,
    Subdivision TEXT,
    Domain TEXT,
    Workgroup TEXT
);)";

    exec(sql);
}

Database::~Database() {
    sqlite3_close(db);
}

Database::Database(Database &&other) noexcept {
        db = other.db;
        other.db = nullptr;
}

Database & Database::operator=(Database &&other) noexcept {
    if (this != &other) {
        db = other.db;
        other.db = nullptr;
    }

    return *this;
}

void Database::exec(std::string_view query, std::function<void(sqlite3_stmt *)> callback_bind) const noexcept(false) {
    sqlite3_stmt *res;


    int rc = sqlite3_prepare_v2(db, query.data(), -1, &res, nullptr);
    if (rc != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(db));


    if (callback_bind)
        callback_bind(res);

    int step = sqlite3_step(res);

    sqlite3_finalize(res);
}
