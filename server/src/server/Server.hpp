//
// Created by helh on 11.04.2026.
//

#pragma once

#include "drogon/HttpResponse.h"
#include <functional>

typedef std::function<void(const drogon::HttpResponsePtr &)> Callback;

namespace server {
void pingIndexHandler(const drogon::HttpRequestPtr &request, Callback &&callback);
void checkHandler(const drogon::HttpRequestPtr &request, Callback &&callback);
drogon::HttpResponsePtr makeSimpleJsonResponse(const std::string &label, const std::string &text, drogon::HttpStatusCode statusCode = drogon::HttpStatusCode::k200OK);
} // server