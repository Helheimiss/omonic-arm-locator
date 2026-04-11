#include "database/Database.hpp"
#include "drogon/HttpAppFramework.h"
#include "utils/Utils.hpp"


int main(int argc, char *argv[]) {

    db = std::make_shared<Database>("db_name");

    drogon::app()
        .addListener("127.0.0.1",8080)
        .enableServerHeader(false)
        .setCustom404Page(Utils::make404Page())
        .run();

    return 0;
}
