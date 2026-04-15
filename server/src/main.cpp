//
// Created by helh on 11.04.2026.
//


#include "armDB/ArmDB.hpp"
#include "drogon/HttpAppFramework.h"
#include "server/Server.hpp"

int main() {

    auto env = std::getenv("OMONIC_ARM_LOCATOR_CONNECTING_STRING");
    if (!env) {
        throw std::runtime_error("env OMONIC_ARM_LOCATOR_CONNECTING_STRING is empty");
    }

    armDB::DB = std::make_unique<armDB::ArmDB>(env);
    armDB::DB->tryCreateTable();

    drogon::app()
        .loadConfigFile("./configs/drogon_config.json")
        .registerHandler("/server/ping", &server::pingIndexHandler, {drogon::Post})
        .registerHandler("/server/check", &server::checkHandler)
        .run();


    return 0;
}
