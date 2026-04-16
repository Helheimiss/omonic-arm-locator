//
// Created by helh on 11.04.2026.
//

#include "Server.hpp"

#include <sys/stat.h>

#include "armDB/ArmDB.hpp"

#include "drogon/HttpAppFramework.h"

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
        static auto rate_limit_ms = drogon::app().getCustomConfig().get("rate_limit_ms", 60000).asInt64();

        callback(makeSimpleJsonResponse("rate_limit_ms", rate_limit_ms));
    }

    void Filter::doFilter(const drogon::HttpRequestPtr &req, drogon::FilterCallback &&fcb, drogon::FilterChainCallback &&fccb) {
        static auto timeout = std::chrono::milliseconds(drogon::app().getCustomConfig().get("rate_limit_ms", 0).asInt());
        auto timenow = std::chrono::steady_clock::now();
        std::string ip = req->getPeerAddr().toIp();


        if (!requests.contains(ip) || requests[ip] < timenow) {
            fccb();

            requests[ip] = timenow + timeout;
        }
        else {
            Json::Value json;
            json["error"] = "timeout";
            json["rate_limit_ms"] = std::chrono::duration_cast<std::chrono::milliseconds>(requests[ip] - timenow).count();

            auto resp = drogon::HttpResponse::newHttpJsonResponse(std::move(json));
            resp->setStatusCode(drogon::HttpStatusCode::k429TooManyRequests);

            fcb(resp);
        }
    }

    drogon::HttpResponsePtr makeSimpleJsonResponse(const std::string &label, const auto value, drogon::HttpStatusCode statusCode) {
        Json::Value json;
        json[label] = value;
        auto resp = drogon::HttpResponse::newHttpJsonResponse(std::move(json));
        resp->setStatusCode(statusCode);
        return resp;
    }
} // server