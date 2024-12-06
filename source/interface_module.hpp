/**
 * @file interface_module.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 06.12.2024
 */

#pragma once

#include "TLA2024.hpp"
#include "i2c_bus.hpp"
#include "gpio.hpp"

#include <thread>
#include <chrono>

class Interface_module {
private:
    TLA2024 * adc;

    GPIO * poe_detect;

    GPIO * poe_budget;

    GPIO * vin_detect;

public:
    Interface_module(I2C_bus *bus);

    ~Interface_module();

    float Voltage_5V();

    float Voltage_Vin();

    float Voltage_PoE();

    float Current();

    float Power_draw();

    bool PoE_status();

    bool PoE_budget();

    bool Vin_status();

private:
    float Measure_voltage(TLA2024::Channels channel);
};
