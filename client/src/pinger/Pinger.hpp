//
// Created by helh on 12.04.2026.
//

#pragma once

#include "httplib.h"
#include "json/json.h"
#include "machineinfo/machineinfo.hpp"
#include <iostream>

class Pinger {
public:
Pinger(std::string host, std::string subdivision) : host(host), subdivision(std::move(subdivision)) {
    client = std::make_unique<httplib::Client>(host);

    client->set_connection_timeout(5, 0);
    client->set_read_timeout(5, 0);
}

int64_t getTimeOut() {
    auto res = client->Get("/server/check");

    std::cout << "client check server" << std::endl;
    if (!res) {
        auto err = res.error();
        std::cerr << httplib::to_string(err) << " " << host << std::endl;

        return 60000 ;
    }


    Json::Value root;
    Json::Reader reader;

    reader.parse(res->body, root);

    int64_t timeout = root.get("rate_limit_ms", 60000).asInt64() + 5000;

    if (res->status == 200) {
        std::cout << "server: ok" << std::endl;
        std::cout << "client: ok" << std::endl;
        std::cout << "server: timeout " << timeout << std::endl;
    }
    else {
        std::cerr << "error by check: " << res->status << std::endl;
    }


    return timeout;
}

void doWork() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(getTimeOut()));
        auto body = createJsonMachineInfo();

        auto res = client->Post("/server/ping", body, "application/json");
        if (!res) {
            auto err = res.error();
            std::cerr << httplib::to_string(err) << " " << host << std::endl;
            std::cout << "client ping" << std::endl;
            std::cout << "server: ok" << std::endl;
        }
        else if (res->status != 200) {
            std::cout << "server: timeout bro" << std::endl;
            std::cerr << "status: " + std::to_string(res->status) << std::endl;
        }
    }
}

private:
std::string createJsonMachineInfo() const {
    auto info = sysinfo::get_machine_info();
    Json::Value root;

    root["UID"] = info.uid;
    root["IP"] = info.ip;
    root["HostName"] = info.hostname;
    root["Subdivision"] = subdivision;
    root["Domain"] = info.domain;
    root["Workgroup"] = info.workgroup;

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream oss;
    writer->write(root, &oss);
    std::string body = oss.str();

    return body;
}


std::unique_ptr<httplib::Client> client;
std::string host;
std::string subdivision;
};
