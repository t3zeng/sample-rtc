#include <iostream>
#include "rtc_driver.h"

int main() {
    RTCDriver rtc;
    
    if (!rtc.initialize()) {
        std::cerr << "RTC initialization failed!" << std::endl;
        return 1;
    }

    uint8_t day = 9;
    uint8_t month = 5;
    uint16_t year = 2023;

    if (!rtc.setDate(day, month, year)) {
        std::cerr << "Setting date failed!" << std::endl;
        return 1;
    }

    uint8_t hours, minutes, seconds;
    if (!rtc.getTime(hours, minutes, seconds)) {
        std::cerr << "Getting time failed!" << std::endl;
        return 1;
    }

    std::cout << "Current time: " << static_cast<int>(hours) << ":"
              << static_cast<int>(minutes) << ":" << static_cast<int>(seconds) << std::endl;

    return 0;
}
