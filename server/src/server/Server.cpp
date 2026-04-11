//
// Created by helh on 11.04.2026.
//

#include "Server.hpp"
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


    // save();


    callback(Utils::makeJson("error", "no error", drogon::HttpStatusCode::k200OK));
    // callback(drogon::HttpResponse::newHttpJsonResponse(*requestBody));
}
