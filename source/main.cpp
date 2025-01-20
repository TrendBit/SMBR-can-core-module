#include "main.hpp"
#include "core_module.hpp"

int main(int argc, char* argv[]) {
    try {
        I2C_bus i2c("/dev/i2c-1");
        Core_module core(&i2c);

        if (argc > 1 && std::string(argv[1]) == "--probe") {
            core.Probe();
        } else {
            core.Run();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
