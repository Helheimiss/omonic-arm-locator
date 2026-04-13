//
// Created by helh on 12.04.2026.
//

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #include <winreg.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <net/if.h>
    #include <ifaddrs.h>
    #include <arpa/inet.h>
    #if defined(__APPLE__)
    #include <sys/sysctl.h>
    #endif
#endif

namespace sysinfo {

struct MachineInfo {
    std::string uid;
    std::string ip;
    std::string hostname;
    std::string domain;
    std::string workgroup;
};

inline MachineInfo get_machine_info() {
    MachineInfo info;

#ifdef _WIN32
    // === Windows ===
    char buf[MAX_COMPUTERNAME_LENGTH + 1] = {0};
    DWORD size = sizeof(buf);

    // HostName
    if (GetComputerNameExA(ComputerNameDnsHostname, buf, &size))
        info.hostname = buf;

    // Domain
    size = sizeof(buf);
    if (GetComputerNameExA(ComputerNameDnsDomain, buf, &size))
        info.domain = buf;

    // Workgroup
    size = sizeof(buf);
    if (GetComputerNameExA(static_cast<COMPUTER_NAME_FORMAT>(5), buf, &size))
        info.workgroup = buf;

    // UID (MachineGuid из реестра)
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char guid[64] = {0};
        DWORD type = REG_SZ, len = sizeof(guid);
        if (RegQueryValueExA(hKey, "MachineGuid", nullptr, &type, reinterpret_cast<LPBYTE>(guid), &len) == ERROR_SUCCESS)
            info.uid = guid;
        RegCloseKey(hKey);
    }

    // IP (первый активный IPv4 не-loopback)
    ULONG outBufLen = 15000;
    std::vector<BYTE> buffer(outBufLen);
    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST |
                             GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME,
                             nullptr, reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data()), &outBufLen) == ERROR_SUCCESS) {
        auto* pAdapter = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buffer.data());
        while (pAdapter) {
            if (pAdapter->OperStatus == IfOperStatusUp && pAdapter->IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
                auto* pUnicast = pAdapter->FirstUnicastAddress;
                while (pUnicast) {
                    if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                        char ipStr[INET_ADDRSTRLEN];
                        auto* addr = reinterpret_cast<sockaddr_in*>(pUnicast->Address.lpSockaddr);
                        inet_ntop(AF_INET, &addr->sin_addr, ipStr, sizeof(ipStr));
                        info.ip = ipStr;
                        pUnicast = nullptr; break;
                    }
                    pUnicast = pUnicast->Next;
                }
                if (!info.ip.empty()) break;
            }
            pAdapter = pAdapter->Next;
        }
    }

#else
    // === Linux / macOS ===
    // HostName
    char hbuf[256] = {0};
    if (gethostname(hbuf, sizeof(hbuf)) == 0)
        info.hostname = hbuf;

    // Domain (DNS domain из resolv.conf)
    std::ifstream resolv("/etc/resolv.conf");
    std::string line;
    while (std::getline(resolv, line)) {
        if (line.find("domain ") == 0 || line.find("search ") == 0) {
            std::istringstream iss(line);
            std::string key; iss >> key;
            iss >> info.domain;
            break;
        }
    }

    // Workgroup (берётся из конфига Samba, если установлен)
    std::ifstream smbconf("/etc/samba/smb.conf");
    if (smbconf.is_open()) {
        while (std::getline(smbconf, line)) {
            if (line.find("workgroup") != std::string::npos && line.find('=') != std::string::npos) {
                auto eq = line.find('=');
                info.workgroup = line.substr(eq + 1);
                info.workgroup.erase(0, info.workgroup.find_first_not_of(" \t\r\n"));
                info.workgroup.erase(info.workgroup.find_last_not_of(" \t\r\n") + 1);
                break;
            }
        }
    }

    // UID
#if defined(__APPLE__)
    // macOS: sysctl kern.osuuid
    size_t len = 0;
    if (sysctlbyname("kern.osuuid", nullptr, &len, nullptr, 0) == 0 && len > 0) {
        std::vector<char> buf(len + 1, 0);
        if (sysctlbyname("kern.osuuid", buf.data(), &len, nullptr, 0) == 0)
            info.uid = buf.data();
    }
#else
    // Linux: /etc/machine-id
    std::ifstream machineid("/etc/machine-id");
    if (machineid.is_open() && std::getline(machineid, info.uid)) {
        // пусто
    } else {
        std::ifstream dbusid("/var/lib/dbus/machine-id");
        if (dbusid.is_open()) std::getline(dbusid, info.uid);
    }
#endif

    // IP (getifaddrs)
    struct ifaddrs* ifap = nullptr;
    if (getifaddrs(&ifap) == 0) {
        for (auto* ifa = ifap; ifa; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                if (!(ifa->ifa_flags & IFF_LOOPBACK)) {
                    char ipStr[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &reinterpret_cast<sockaddr_in*>(ifa->ifa_addr)->sin_addr, ipStr, sizeof(ipStr));
                    info.ip = ipStr;
                    break;
                }
            }
        }
        freeifaddrs(ifap);
    }
#endif

    return info;
}

} // namespace sysinfo
