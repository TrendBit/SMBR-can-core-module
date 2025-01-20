#include "core_module.hpp"
#include <iostream>
#include <iomanip>

Core_module::Core_module(I2C_bus* bus):
    can_interface(new CAN::Interface("can0")),
    interface(new Interface_board(bus)),
    rpi(new RPi_host())
{}

Core_module::~Core_module() {
    delete interface;
    delete rpi;
}

void Core_module::Probe() const {
    std::cout << "5V sense: " << std::fixed << std::setprecision(3) << interface->Voltage_5V() << " V" << std::endl;
    std::cout << "Vin sense: " << std::fixed << std::setprecision(3) << interface->Voltage_Vin() << " V" << std::endl;
    std::cout << "PoE sense: " << std::fixed << std::setprecision(3) << interface->Voltage_PoE() << " V" << std::endl;
    std::cout << "Current sense: " << std::fixed << std::setprecision(3) << interface->Current() << " A" << std::endl;
    std::cout << "Power draw: " << std::fixed << std::setprecision(3) << interface->Power_draw() << " W" << std::endl;
    bool poe_status = interface->PoE_status();
    std::cout << "PoE status: " << (poe_status ? "Connected" : "Disconnected") << std::endl;
    if (poe_status) {
        std::cout << "PoE budget: " << (interface->PoE_budget() ? "Basic" : "Plus") << std::endl;
    }
    std::cout << "Vin status: " << (interface->Vin_status() ? "Connected" : "Disconnected") << std::endl;
    std::cout << "Board temperature: " << std::fixed << std::setprecision(1) << interface->Board_temperature() << " °C" << std::endl;
    std::cout << "Core temperature: " << std::fixed << std::setprecision(1) << rpi->Core_temperature() << " °C" << std::endl;
    std::cout << "Core load: " << std::fixed << std::setprecision(1) << rpi->Core_load() << " %" << std::endl;

    // Print UID
    auto uid = rpi->Device_UID();
    std::cout << "Device UID: 0x";
    for (auto byte : uid) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;

    // Print Short ID
    std::cout << "Short ID: 0x" << std::setfill('0') << std::setw(4)
              << std::hex << rpi->Short_ID() << std::dec << std::endl;

    // Print IP address
    auto ip = rpi->IP_address();
    if (ip.has_value()) {
        std::cout << "IP Address: ";
        for (size_t i = 0; i < ip.value().size(); i++) {
            std::cout << static_cast<int>(ip.value()[i]);
            if (i < ip.value().size() - 1) std::cout << ".";
        }
        std::cout << std::endl;
    } else {
        std::cout << "IP Address: Not available" << std::endl;
    }

    std::cout << "Hostname: " << rpi->Hostname() << std::endl;
}

void Core_module::Run() {
    std::cout << "Starting CAN message processing" << std::endl;
    while(true){

        Application_message message = can_interface->Receiver_loop();
        auto message_type = message.Message_type();

        switch (message_type) {
            case Codes::Message_type::Probe_modules_request:{
                std::cout << "Module probe request received" << std::endl;
                App_messages::Common::Probe_modules_response probe_response(rpi->Device_UID());
                can_interface->Send_message(probe_response);
                break;
            }

            case Codes::Message_type::Ping_request:{
                std::cout << "Ping request received" << std::endl;
                App_messages::Common::Ping_request ping_request;
                if (!ping_request.Interpret_data(message.data)){
                    std::cout << "Ping request data interpretation failed" << std::endl;
                    break;
                }
                uint8_t sequence_number = ping_request.sequence_number;
                App_messages::Common::Ping_response ping_response(sequence_number);
                can_interface->Send_message(ping_response);
                break;
            }

            case Codes::Message_type::Core_load_request:{
                std::cout << "Core load request received" << std::endl;
                App_messages::Common::Core_load_response core_load_response(rpi->Core_load());
                can_interface->Send_message(core_load_response);
                break;
            }

            case Codes::Message_type::Core_temperature_request:{
                std::cout << "Core temperature request received" << std::endl;
                App_messages::Common::Core_temp_response core_temp_response(rpi->Core_temperature());
                can_interface->Send_message(core_temp_response);
                break;
            }

            case Codes::Message_type::Board_temperature_request:{
                std::cout << "Board temperature request received" << std::endl;
                App_messages::Common::Board_temp_response board_temp_response(interface->Board_temperature());
                can_interface->Send_message(board_temp_response);
                break;
            }



            default:
                std::cout << "Unknown message received" << std::endl;
                break;
        }
    }
}
