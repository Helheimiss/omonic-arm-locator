//
// Created by helh on 11.04.2026.
//

#include <QCoreApplication>
#include "drogon/HttpAppFramework.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);


    drogon::app()
        .addListener("127.0.0.1",8080)
        .enableServerHeader(false)
        // .setCustom404Page(Utils::make404Page())
        .run();

    return app.exec();
}
