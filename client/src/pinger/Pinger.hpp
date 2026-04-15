#pragma once

#include "httplib.h"
#include "json/json.h"
#include "machineinfo/machineinfo.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

class Pinger {
public:
    Pinger(const std::string& host, std::string subdivision)
        : host(host), subdivision(std::move(subdivision)) {
        client = std::make_unique<httplib::Client>(host);

        // Таймауты лучше вынести в конфиг, но 5с — разумно
        client->set_connection_timeout(5, 0);
        client->set_read_timeout(5, 0);
    }

    int64_t getTimeOut() {
        auto res = client->Get("/server/check");

        if (!res) {
            logError("check", res.error());
            return DEFAULT_TIMEOUT;
        }

        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errs;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());


        bool parsingSuccessful = reader->parse(
            res->body.c_str(),
            res->body.c_str() + res->body.size(),
            &root,
            &errs
        );

        int64_t timeout = DEFAULT_TIMEOUT;

        if (parsingSuccessful && root.isObject()) {
            timeout = root.get("rate_limit_ms", (Json::Int64)DEFAULT_TIMEOUT).asInt64() + 5000;
        } else {
            std::cerr << "JSON Parse error or not an object: " << errs << std::endl;
        }

        if (res->status == 200) {
            std::cout << "Server status: OK | Timeout: " << timeout << "ms" << std::endl;
        } else {
            std::cerr << "Server returned error: " << res->status << std::endl;
        }

        return timeout;
    }

    void doWork() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(getTimeOut()));

            auto body = createJsonMachineInfo();
            auto res = client->Post("/server/ping", body, "application/json");

            if (!res) {
                logError("ping", res.error());
            } else if (res->status != 200) {
                std::cerr << "Ping failed. Status: " << res->status << " Body: " << res->body << std::endl;
            } else {
                std::cout << "Ping success" << std::endl;
            }
        }
    }

private:
    void logError(const std::string& op, httplib::Error err) const {
        std::cerr << "Operation [" << op << "] failed: " << httplib::to_string(err)
                  << " Host: " << host << std::endl;
    }

    std::string createJsonMachineInfo() const {
        auto info = sysinfo::get_machine_info();
        Json::Value root;

        root["UID"] = info.uid;
        root["IP"] = info.ip;
        root["HostName"] = info.hostname;
        root["Subdivision"] = subdivision;
        root["Domain"] = info.domain;
        root["Workgroup"] = info.workgroup;

        // Компактный вывод JSON (без лишних пробелов)
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";
        return Json::writeString(builder, root);
    }

    const int64_t DEFAULT_TIMEOUT = 60000;
    std::unique_ptr<httplib::Client> client;
    std::string host;
    std::string subdivision;
};
