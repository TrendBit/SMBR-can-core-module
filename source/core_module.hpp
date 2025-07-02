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
#include "can/can_interface.hpp"
#include "can/app_message.hpp"

#include "codes/messages/common/ping_request.hpp"
#include "codes/messages/common/ping_response.hpp"
#include "codes/messages/common/probe_modules_response.hpp"
#include "codes/messages/common/core_load_response.hpp"
#include "codes/messages/common/core_temp_response.hpp"
#include "codes/messages/common/board_temp_response.hpp"
#include "codes/messages/core/sid_response.hpp"
#include "codes/messages/core/ip_response.hpp"
#include "codes/messages/core/hostname_response.hpp"
#include "codes/messages/core/serial_response.hpp"
#include "codes/messages/core/supply_type_response.hpp"
#include "codes/messages/core/supply_5v_rail_response.hpp"
#include "codes/messages/core/supply_vin_rail_response.hpp"
#include "codes/messages/core/supply_poe_rail_response.hpp"
#include "codes/messages/core/supply_current_response.hpp"
#include "codes/messages/core/supply_power_draw_response.hpp"
#include "codes/messages/common/fw_version_response.hpp"
#include "codes/messages/common/fw_hash_response.hpp"
#include "codes/messages/common/fw_dirty_response.hpp"
#include "codes/messages/common/hw_version_response.hpp"
#include "codes/messages/core/model_response.hpp"

/**
 * @brief   Firmware version information - Major version number
 */
#ifndef FW_VERSION_MAJOR
    #define FW_VERSION_MAJOR 0
#endif

/**
 * @brief   Firmware version information - Minor version number
 */
#ifndef FW_VERSION_MINOR
    #define FW_VERSION_MINOR 0
#endif

/**
 * @brief   Firmware version information - Patch version number
 */
#ifndef FW_VERSION_PATCH
    #define FW_VERSION_PATCH 0
#endif

/**
 * @brief   Firmware version information - Git commit hash as uint64_t
 */
#ifndef FW_GIT_COMMIT_HASH_HEX
    #define FW_GIT_COMMIT_HASH_HEX 0x0000000
#endif

#ifndef FW_GIT_DIRTY
    #define FW_GIT_DIRTY true
#endif

/**
 * @brief   Core module class representing main application logic
 */
class Core_module {
private:

    /**
     * @brief  Structure holding firmware version information passed by compiler based on git tags
     */
    const struct{
        uint16_t major = FW_VERSION_MAJOR;
        uint16_t minor = FW_VERSION_MINOR;
        uint16_t patch = FW_VERSION_PATCH;
    } fw_version ;

    /**
     * @brief Actual commit hash of firmware as uint64_t
     */
    const uint64_t fw_hash = FW_GIT_COMMIT_HASH_HEX;

    /**
     * @brief Flag indicating if firmware repository is dirty (modified)
     */
    const bool fw_dirty = FW_GIT_DIRTY;

    /**
     * @brief  CAN interface for sending and receiving messages from CAN BUS
     */
    CAN::Interface *const can_interface;

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
     * @brief   Test function for hardware verification, will try to execute all data suppliers and print their values
     */
    void Probe() const;

    /**
     * @brief   Main loop of core module, which is waiting for messages from CAN BUS and process them
     */
    void Run();

};
