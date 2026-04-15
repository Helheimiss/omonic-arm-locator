//
// Created by helh on 11.04.2026.
//

#pragma once

#include "drogon/drogon_callbacks.h"
#include "drogon/HttpResponse.h"
#include <functional>

#include "drogon/HttpFilter.h"


typedef std::function<void(const drogon::HttpResponsePtr &)> Callback;

namespace server {
void pingIndexHandler(const drogon::HttpRequestPtr &request, Callback &&callback);
void checkHandler(const drogon::HttpRequestPtr &request, Callback &&callback);

class Filter : public drogon::HttpFilter<Filter> {
public:
    void doFilter(const drogon::HttpRequestPtr &req, drogon::FilterCallback &&fcb, drogon::FilterChainCallback &&fccb) override;

private:
    std::unordered_map<std::string, std::chrono::time_point<std::chrono::steady_clock>> requests;
};

drogon::HttpResponsePtr makeSimpleJsonResponse(const std::string &label, const auto &text, drogon::HttpStatusCode statusCode = drogon::HttpStatusCode::k200OK);
} // server