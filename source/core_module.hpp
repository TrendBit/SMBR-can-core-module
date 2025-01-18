/**
 * @file core_module.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @brief
 * @version 0.1
 * @date 06.12.2024
 */

#pragma once

#include "interface_board.hpp"
#include "rpi_host.hpp"
#include "can/can_receiver.hpp"
#include "can/app_message.hpp"

#include "codes/messages/common/probe_modules_response.hpp"

/**
 * @brief   Core module class representing main application logic
 */
class Core_module {
private:

    /**
     * @brief  CAN receiver for receiving messages from CAN BUS
     */
    CAN::Receiver *const can_receiver;

    /**
     * @brief   Interface board hardware abstraction
     */
    Interface_board *const interface;

    /**
     * @brief   Raspberry Pi host controller
     */
    RPi_host *const rpi;

public:

    /**
     * @brief   Construct a new Core module object
     * @param   bus I2C bus instance for hardware communication
     */
    explicit Core_module(I2C_bus *bus);

    /**
     * @brief   Destroy the Core module object
     */
    ~Core_module();

    /**
     * @brief   Test function for hardware verification
     */
    void Test() const;

    void Run();
};
