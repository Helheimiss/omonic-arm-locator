//
// Created by helh on 11.04.2026.
//

#include "Server.hpp"

#include "database/Database.hpp"
#include "utils/Utils.hpp"

void Server::ping(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto requestBody = req->getJsonObject();

    if (!requestBody) {
        auto resp = Utils::makeJson("error", "empty request body");
        callback(resp);
        return;
    }

    auto UID = requestBody->get("UID", "").asString();
    auto Date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto IP = requestBody->get("IP", "").asString();
    auto HostName = requestBody->get("HostName", "").asString();
    auto Subdivision = requestBody->get("Subdivision", "").asString();
    auto Domain = requestBody->get("Domain", "").asString();
    auto Workgroup = requestBody->get("Workgroup", "").asString();


    const char *sql =
R"(
INSERT INTO device_logs (UID, Date, IP, HostName, Subdivision, Domain, Workgroup)
VALUES (?, ?, ?, ?, ?, ?, ?)
ON CONFLICT(UID) DO UPDATE SET
    Date = excluded.Date,
    IP = excluded.IP,
    HostName = excluded.HostName,
    Subdivision = excluded.Subdivision,
    Domain = excluded.Domain,
    Workgroup = excluded.Workgroup;)";


    // save();
    db->exec(sql, [&](auto stmt) {
        sqlite3_bind_text(stmt, 1, UID.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, Date);
        sqlite3_bind_text(stmt, 3, IP.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, HostName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, Subdivision.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, Domain.c_str(), -1, SQLITE_STATIC);

    });


    callback(Utils::makeJson("error", "no error", drogon::HttpStatusCode::k200OK));
    // callback(drogon::HttpResponse::newHttpJsonResponse(*requestBody));
}
