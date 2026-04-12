//
// Created by helh on 11.04.2026.
//

#include "Server.hpp"

#include "armDB/ArmDB.hpp"

namespace server {
    void pingIndexHandler(const drogon::HttpRequestPtr &request, Callback &&callback) {
        auto requestBody = request->getJsonObject();

        if (!requestBody) {
            auto json = makeSimpleJsonResponse("error", "empty request body", drogon::HttpStatusCode::k400BadRequest);
            callback(json);
            return;
        }


        auto UID = QString::fromStdString(requestBody->get("UID", "NULL").asString());
        auto IP = QString::fromStdString(requestBody->get("IP", "NULL").asString());
        auto HostName = QString::fromStdString(requestBody->get("HostName", "NULL").asString());
        auto Subdivision = QString::fromStdString(requestBody->get("Subdivision", "NULL").asString());
        auto Domain = QString::fromStdString(requestBody->get("Domain", "NULL").asString());
        auto Workgroup = QString::fromStdString(requestBody->get("Workgroup", "NULL").asString());

        armDB::DB->tryInsertLogsToDB(UID, IP, HostName, Subdivision, Domain, Workgroup);

        callback(makeSimpleJsonResponse("error", "no errors"));
    }

    drogon::HttpResponsePtr makeSimpleJsonResponse(const std::string &label, const std::string &text, drogon::HttpStatusCode statusCode) {
        Json::Value json;
        json[label] = text;
        return drogon::HttpResponse::newHttpJsonResponse(std::move(json));
    }
} // server