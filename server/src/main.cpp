//
// Created by helh on 11.04.2026.
//

#include <QCoreApplication>

#include "armDB/ArmDB.hpp"
#include "drogon/HttpAppFramework.h"
#include "server/Server.hpp"

#include <fstream>

void loadConfig() {
    using namespace std::string_literals;
    std::ifstream file("./configs/database.json");
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;

    if (!Json::parseFromStream(builder, file, &root, &errs)) {
        throw std::runtime_error("Error parsing JSON: "s + errs);
    }

    auto Type = root.get("Type", "").asString();
    auto HostName = root.get("HostName", "").asString();
    auto DatabaseName = root.get("DatabaseName", "").asString();
    auto UserName = root.get("UserName", "").asString();
    auto Password  = root.get("Password", "").asString();
    auto ConnectOptions = root.get("ConnectOptions", "").asString();
    auto Port  = root.get("Port", "3306").asInt();

    armDB::DB = std::make_unique<armDB::ArmDB>
    (
        QString::fromStdString(Type),
        QString::fromStdString(HostName),
        QString::fromStdString(DatabaseName),
        QString::fromStdString(UserName),
        QString::fromStdString(Password),
        QString::fromStdString(ConnectOptions),
        Port
    );
}


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);


    loadConfig();
    armDB::DB->tryCreateTable();


    drogon::app()
        .loadConfigFile("./configs/server.json")
        .registerHandler("/server/ping", &server::pingIndexHandler, {drogon::Post})
        .registerHandler("/server/check", &server::checkHandler)
        .run();

    return app.exec();
}
