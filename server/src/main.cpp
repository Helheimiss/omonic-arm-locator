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


    drogon::app()
        .loadConfigFile("./configs/drogon_config.json")
        .enableServerHeader(false)
        .setCustom404Page(server::makeSimpleJsonResponse("error", "404", drogon::k404NotFound))
        .registerHandler("/server/ping", &server::pingIndexHandler, {drogon::Post, "server::Filter"})
        .registerHandler("/server/check", &server::checkHandler, {drogon::Get, "server::Filter"});


    armDB::DB = std::make_unique<armDB::ArmDB>(
        env, drogon::app().getCustomConfig().get("table_name", "omn_arm_locator_logs").asString());
    armDB::DB->tryCreateTable();


    drogon::app().run();
    return 0;
}
