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
Pinger(std::string host, std::string subdivision, int awaitInMin) : host(host), subdivision(subdivision), awaitInMin(awaitInMin) {
    client = std::make_unique<httplib::Client>(host);

    client->set_connection_timeout(5, 0);
    client->set_read_timeout(5, 0);
}

void doWork() {
    auto res = client->Get("/server/check");

    std::cout << "client check server" << std::endl;
    if (!res) {
        auto err = res.error();
        std::cerr << httplib::to_string(err) << " " << host << std::endl;

    }
    else if (res->status == 200) {
        std::cout << "server: ok" << std::endl;
        std::cout << "client: ok" << std::endl;
    }
    else {
        std::cerr << res->status << std::endl;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::minutes(awaitInMin));
        auto body = createJsonMachineInfo();

        auto res = client->Post("/server/ping", body, "application/json");
        if (!res) {
            auto err = res.error();
            std::cerr << httplib::to_string(err) << " " << host << std::endl;
            std::cout << "client ping" << std::endl;
            std::cout << "server: ok" << std::endl;
        }
        else if (res->status != 200) {
            std::cout << "server: bad" << std::endl;
            std::cerr << "status: " + res->status << std::endl;
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

void ping() {
    auto json = createJsonMachineInfo();
}

std::unique_ptr<httplib::Client> client;
std::string host;
std::string subdivision;
int awaitInMin;
};
