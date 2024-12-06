#include "main.hpp"

int main() {
    try {
        // Open the I2C bus
        I2C_bus i2c("/dev/i2c-1");

        auto interface_module = new Interface_module(&i2c);

        std::cout << "5V sense: " << std::fixed << std::setprecision(3) << interface_module->Voltage_5V() << " V" << std::endl;
        std::cout << "Vin sense: " << std::fixed << std::setprecision(3) << interface_module->Voltage_Vin() << " V" << std::endl;
        std::cout << "PoE sense: " << std::fixed << std::setprecision(3) << interface_module->Voltage_PoE() << " V" << std::endl;
        std::cout << "Current sense: " << std::fixed << std::setprecision(3) << interface_module->Current() << " A" << std::endl;
        std::cout << "Power draw: " << std::fixed << std::setprecision(3) << interface_module->Power_draw() << " W" << std::endl;
        bool poe_status = interface_module->PoE_status();
        std::cout << "PoE status: " << (poe_status ? "Connected" : "Disconnected") << std::endl;
        if (poe_status) {
            std::cout << "PoE budget: " << (interface_module->PoE_budget() ? "Basic" : "Plus") << std::endl;
        }
        std::cout << "Vin status: " << (interface_module->Vin_status() ? "Connected" : "Disconnected") << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
