//
// Created by helh on 12.04.2026.
//

#include <pinger/Pinger.hpp>

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cerr << "Usage:\t" << argv[0] << " <hostname> <subdivision>" << std::endl;
        std::cerr << "\texample:\t" << argv[0] << " 192.168.0.5:8080 \"дом ученого\"" << std::endl;

        return 1;
    }

    Pinger pinger(argv[1], argv[2]);
    pinger.doWork();

    return 0;
}
