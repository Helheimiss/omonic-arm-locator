//
// Created by helh on 11.04.2026.
//

#include <QCoreApplication>

#include "armDB/ArmDB.hpp"
#include "drogon/HttpAppFramework.h"
#include "server/Server.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    armDB::DB = std::make_unique<armDB::ArmDB>


    armDB::DB->tryCreateTable();

    drogon::app()
        .addListener("127.0.0.1",8080)
        .enableServerHeader(false)
        // .setCustom404Page(Utils::make404Page())
        .registerHandler("/server/ping", &server::pingIndexHandler, {drogon::Post})
        .run();

    return app.exec();
}
