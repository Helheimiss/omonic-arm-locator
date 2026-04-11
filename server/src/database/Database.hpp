//
// Created by helh on 11.04.2026.
//

#ifndef OMONIC_ARM_LOCATOR_SERVER_DATABASE_HPP
#define OMONIC_ARM_LOCATOR_SERVER_DATABASE_HPP

#include <sqlite3.h>
#include <functional>
#include <string>

class Database {
public:
    explicit Database(std::string_view path) noexcept(false);
    ~Database();

    Database(const Database &other) = delete;
    Database & operator=(const Database &other) = delete;

    Database(Database &&other) noexcept;
    Database & operator=(Database &&other) noexcept;

    void exec(std::string_view query, std::function<void(sqlite3_stmt *)> callback_bind=nullptr) noexcept(false);

private:
    sqlite3 *db;
};

inline Database db("test.db");

#endif //OMONIC_ARM_LOCATOR_SERVER_DATABASE_HPP
