#ifndef MOCK_PCF8563_H
#define MOCK_PCF8563_H

#include <cstdint>
#include <cstddef>

#define PCF8563_WRITE_ADDRESS 0xA2
#define PCF8563_READ_ADDRESS 0xA3

#define CONTROL_STATUS1_REG 0x00
#define CONTROL_STATUS2_REG 0x01
#define SECONDS_REG 0x02
#define MINUTES_REG 0x03
#define HOURS_REG 0x04
#define DAYS_REG 0x05
#define WEEKDAYS_REG 0x06
#define CENTURY_MONTHS_REG 0x07
#define YEARS_REG 0x08

class MockPCF8563 {
private:
    uint8_t _controlStatus1;
    uint8_t _controlStatus2;
    uint8_t _seconds;
    uint8_t _minutes;
    uint8_t _hours;
    uint8_t _days;
    uint8_t _weekdays;
    uint8_t _centuryMonths;
    uint8_t _years;

public:
    bool initialize();

    bool readRegisterByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data);

    bool writeRegisterByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t data);

    bool readFromRegister(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data, size_t dataSize);

    bool writeToRegister(uint8_t slaveAddress, uint8_t registerAddress, const uint8_t* data, size_t dataSize);
};

#endif // MOCK_PCF8563_H
