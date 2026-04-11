//
// Created by helh on 11.04.2026.
//

#pragma once

#include "drogon/HttpController.h"

class Server : public drogon::HttpController<Server> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Server::ping, "/ping", drogon::Post);
    METHOD_LIST_END

    void ping(const drogon::HttpRequestPtr &req, std::function<void (const drogon::HttpResponsePtr &)> &&callback);
};