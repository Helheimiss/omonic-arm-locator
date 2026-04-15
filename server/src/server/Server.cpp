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


        auto UID = requestBody->get("UID", "NULL").asString();
        auto IP = request->getPeerAddr().toIp();
        auto HostName = requestBody->get("HostName", "NULL").asString();
        auto Subdivision = requestBody->get("Subdivision", "NULL").asString();
        auto Domain = requestBody->get("Domain", "NULL").asString();
        auto Workgroup = requestBody->get("Workgroup", "NULL").asString();

        armDB::DB->tryInsertLogsToDB(UID, IP, HostName, Subdivision, Domain, Workgroup);

        callback(makeSimpleJsonResponse("error", "no errors"));
    }

    void checkHandler(const drogon::HttpRequestPtr &request, Callback &&callback) {


        callback(makeSimpleJsonResponse("error", "no errors"));
    }

    drogon::HttpResponsePtr makeSimpleJsonResponse(const std::string &label, const std::string &text, drogon::HttpStatusCode statusCode) {
        Json::Value json;
        json[label] = text;
        return drogon::HttpResponse::newHttpJsonResponse(std::move(json));
    }
} // server