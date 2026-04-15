//
// Created by helh on 11.04.2026.
//


#include "armDB/ArmDB.hpp"
#include "drogon/HttpAppFramework.h"
#include "server/Server.hpp"

int main(int argc, char *argv[]) {
    armDB::DB->tryCreateTable();


    drogon::app()
        .loadConfigFile("./configs/server.json")
        .registerHandler("/server/ping", &server::pingIndexHandler, {drogon::Post})
        .registerHandler("/server/check", &server::checkHandler)
        .run();

    return 0;
}
