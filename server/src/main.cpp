#include "database/SqlLite3Database.hpp"
#include "drogon/HttpAppFramework.h"
#include "utils/Utils.hpp"

#include <QCoreApplication>

#include "database/Database.hpp"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    db = std::make_shared<SqlLite3Database>("device_logs.db");

    Database dd
    dd.tryCreateDB();

    drogon::app()
        .addListener("127.0.0.1",8080)
        .enableServerHeader(false)
        .setCustom404Page(Utils::make404Page())
        .run();

    return app.exec();
}
