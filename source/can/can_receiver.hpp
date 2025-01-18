/**
 * @file can_receiver.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 18.01.2025
 */

#pragma once

#include <cstring>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <optional>

#include "codes/codes.hpp"
#include "can/can_message.hpp"
#include "can/app_message.hpp"

namespace CAN {

class Receiver {
private:
    int socket_fd;

    const Codes::Module module_type = Codes::Module::Core_module;

    const Codes::Instance instance = Codes::Instance::Exclusive;

    std::vector<Codes::Message_type> accepted_messages = {
        Codes::Message_type::Probe_modules_request,
    };

public:
    Receiver(const std::string& interface);

    ~Receiver();

    Application_message Receiver_loop();

    /**
     * @brief   Send Application message via CAN bus
     * @param   message Application message to send
     * @return  bool True if successful
     */
    bool Send_message(const Application_message& message);

private:

    std::optional<Codes::Message_type> Process_message(Application_message &app_message);

    /**
     * @brief   Initialize CAN socket
     * @param   interface CAN interface name
     * @return  true if successful
     */
    bool Init_socket(const std::string& interface);
};

}
