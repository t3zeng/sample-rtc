#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include <cstdint>

class RTCDriver {
public:
    RTCDriver();
    bool initialize();
    bool getTime(uint8_t& hours, uint8_t& minutes, uint8_t& seconds);
    bool setDate(uint8_t day, uint8_t month, uint16_t year);
};

#endif
