#include "main.hpp"
#include "core_module.hpp"

int main() {
    try {
        I2C_bus i2c("/dev/i2c-1");
        Core_module core(&i2c);
        core.Test();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
