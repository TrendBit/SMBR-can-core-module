#pragma once

#include <stdexcept>
#include <cstdint>

class GPIO {
private:
    int gpio_pin;
    uint32_t *gpio_base;
    static const uint32_t GPIO_BASE = 0x3F200000; // For Raspberry Pi 3/4
    static const uint32_t GPIO_LEN = 0xB4;       // Length of GPIO memory area

public:
    enum class Direction {
        In,
        Out
    };

    explicit GPIO(int pin, Direction dir = Direction::In);
    ~GPIO();

    void Set_direction(Direction dir);
    bool Read() const;
    void Set_value(bool value) const;

private:
    void *gpio_map;
    int mem_fd;

    void Init_GPIO();
    void Deinit_GPIO();
};
