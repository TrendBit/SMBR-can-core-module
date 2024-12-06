#include "interface_module.hpp"

Interface_module::Interface_module(I2C_bus *bus):
    adc(new TLA2024(*bus, 0x48))
{
    poe_detect = new GPIO(22, GPIO::Direction::In);
    poe_budget = new GPIO(27, GPIO::Direction::In);
    vin_detect = new GPIO(17, GPIO::Direction::In);
}

Interface_module::~Interface_module()
{
    delete adc;
    delete poe_detect;
    delete poe_budget;
    delete vin_detect;

}

float Interface_module::Voltage_5V()
{
    float divider = (20.0+10.0)/10.0;

    return Measure_voltage(TLA2024::Channels::AIN0_GND) * divider;
}

float Interface_module::Voltage_Vin()
{
    float divider = (100.0+10.0)/10.0;

    return Measure_voltage(TLA2024::Channels::AIN2_GND) * divider;
}

float Interface_module::Voltage_PoE()
{
    float divider = (100.0+10.0)/10.0;

    return Measure_voltage(TLA2024::Channels::AIN3_GND) * divider;
}

float Interface_module::Current()
{
    float amplification = 50.0;

    float sense_resistor = 0.01;

    float voltage = Measure_voltage(TLA2024::Channels::AIN1_GND);

    float sense_voltage = voltage / amplification;

    float current = sense_voltage / sense_resistor;

    return current;
}

bool Interface_module::PoE_status()
{
    return not poe_detect->Read();
}

bool Interface_module::PoE_budget()
{
    return not poe_budget->Read();
}

bool Interface_module::Vin_status()
{
    return not vin_detect->Read();
}

float Interface_module::Power_draw()
{
    float voltage = Voltage_Vin();
    float current = Current();

    return voltage * current;
}

float Interface_module::Measure_voltage(TLA2024::Channels channel){
    adc->Select_channel(channel);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    float voltage = adc->Read_voltage().value_or(0.0f);

    return voltage;
}
