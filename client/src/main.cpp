//
// Created by helh on 12.04.2026.
//

#include <pinger/Pinger.hpp>

int main(int argc, char *argv[]) {

    if (argc < 4) {
        std::cerr << "Usage:\t" << argv[0] << " <hostname> <subdivision> <await in min>" << std::endl;
        std::cerr << "\texample:\t" << argv[0] << " 192.168.0.5:8080 \"дом ученого\" 5" << std::endl;

        return 1;
    }

    Pinger pinger(argv[1], argv[2], std::stoi(argv[3]));
    pinger.doWork();

    return 0;
}
