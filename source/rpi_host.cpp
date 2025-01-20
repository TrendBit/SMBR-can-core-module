#include "rpi_host.hpp"

float RPi_host::Core_temperature() {
    std::ifstream temp_file("/sys/class/thermal/thermal_zone0/temp");
    if (!temp_file.is_open()) {
        return 0.0f;
    }

    int temp;
    temp_file >> temp;
    return static_cast<float>(temp) / 1000.0f;
}

float RPi_host::Core_load() {
    std::ifstream loadavg("/proc/loadavg");
    if (!loadavg.is_open()) {
        return 0.0f;
    }

    float load;
    loadavg >> load;
    return load;
}

std::string RPi_host::Read_serial() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;

    while (std::getline(cpuinfo, line)) {
        if (line.find("Serial") != std::string::npos) {
            return line.substr(line.find(":") + 2);
        }
    }
    return "";
}

std::array<uint8_t, 6> RPi_host::Hash(const std::string& input) {
    // Use std::hash instead of SHA256
    std::hash<std::string> hasher;
    size_t hash_value = hasher(input);

    std::array<uint8_t, 6> result;
    result[0] = (hash_value >> 40) & 0xFF;
    result[1] = (hash_value >> 32) & 0xFF;
    result[2] = (hash_value >> 24) & 0xFF;
    result[3] = (hash_value >> 16) & 0xFF;
    result[4] = (hash_value >> 8) & 0xFF;
    result[5] = hash_value & 0xFF;
    return result;
}

std::array<uint8_t, 6> RPi_host::Device_UID() {
    std::string serial = Read_serial();
    if (serial.empty()) {
        return {0, 0, 0, 0, 0, 0};
    }

    // Use last 4 bytes of serial + "RPi4B" as salt
    std::string input = serial.substr(serial.length() - 8) + "RPi4B";
    return Hash(input);
}

uint16_t RPi_host::Short_ID() {
    auto uid = Device_UID();
    // XOR first 3 bytes with last 3 bytes for better distribution
    uint16_t short_id = ((uid[0] ^ uid[3]) << 8) | (uid[1] ^ uid[4]);
    return short_id;
}

uint32_t RPi_host::Serial_number() {
    std::string serial = Read_serial();
    if (serial.empty()) {
        return 0;
    }

    // Convert hex string to integer
    return std::stoul(serial, nullptr, 16);
}

std::optional<std::array<uint8_t, 4>> RPi_host::IP_address() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        return {};
    }

    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;

    const char* interfaces[] = {"eth0", "wlan0"};
    for (const auto& interface : interfaces) {
        strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
        if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
            close(fd);
            struct sockaddr_in* addr = (struct sockaddr_in*)&ifr.ifr_addr;
            uint32_t ip = addr->sin_addr.s_addr;
            return std::array<uint8_t, 4>{
                static_cast<uint8_t>(ip & 0xFF),
                static_cast<uint8_t>((ip >> 8) & 0xFF),
                static_cast<uint8_t>((ip >> 16) & 0xFF),
                static_cast<uint8_t>((ip >> 24) & 0xFF)
            };
        }
    }

    close(fd);
    return {};
}

std::string RPi_host::Hostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        std::string host(hostname);
        return host.substr(0, 8);  // Limit to 8 characters
    }
    return "";
}
