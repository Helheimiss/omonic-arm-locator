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

    auto IP = requestBody->get("IP", "N/A").asString();
    auto HostName = requestBody->get("HostName", "N/A").asString();
    auto Subdivision = requestBody->get("Subdivision", "N/A").asString();
    auto Domain = requestBody->get("Domain", "N/A").asString();
    auto Workgroup = requestBody->get("Workgroup", "N/A").asString();


    // save();


    callback(Utils::makeJson("error", "no error", drogon::HttpStatusCode::k200OK));
    callback(drogon::HttpResponse::newHttpJsonResponse(*requestBody));
}
