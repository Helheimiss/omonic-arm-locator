//
// Created by helh on 11.04.2026.
//
#pragma once

#include "drogon/HttpResponse.h"

namespace Utils {
inline drogon::HttpResponsePtr makeJson(const std::string &label, const std::string &text, drogon::HttpStatusCode statusCode=drogon::HttpStatusCode::k404NotFound) {
    Json::Value json;
    json[label] = text;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(statusCode);

    return resp;
}

inline drogon::HttpResponsePtr make404Page() {
    return makeJson("error", "not found");
}
}