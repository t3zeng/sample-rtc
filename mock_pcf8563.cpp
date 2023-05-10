#include <time.h>
#include "mock_pcf8563.h"

static uint8_t bcdFromDecimal(uint8_t decimalValue) {
    return ((decimalValue / 10) << 4) | (decimalValue % 10);
}

bool MockPCF8563::initialize() {
    // get current time
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    // Initialize the RTC to default values
    _controlStatus1 = 0x00;
    _controlStatus2 = 0x00;
    _seconds = bcdFromDecimal(info->tm_sec);
    _minutes = bcdFromDecimal(info->tm_min);
    _hours = bcdFromDecimal(info->tm_hour);
    _days = bcdFromDecimal(info->tm_mday);
    _weekdays = info->tm_wday;
    _centuryMonths = bcdFromDecimal(info->tm_mon) | (uint8_t)(info->tm_year >= 100) << 7;
    _years = bcdFromDecimal(info->tm_year % 100);

    return true;
}

bool MockPCF8563::readRegisterByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data) {
    // error out if the read address is wrong
    if (slaveAddress != PCF8563_READ_ADDRESS) {
        return false;
    }

    switch (registerAddress) {
        case CONTROL_STATUS1_REG:
            *data = _controlStatus1;
            break;
        case CONTROL_STATUS2_REG:
            *data = _controlStatus1;
            break;
        case SECONDS_REG:
            *data = _seconds;
            break;
        case MINUTES_REG:
            *data = _minutes;
            break;
        case HOURS_REG:
            *data = _hours;
            break;
        case DAYS_REG:
            *data = _days;
            break;
        case WEEKDAYS_REG:
            *data = _weekdays;
            break;
        case CENTURY_MONTHS_REG:
            *data = _centuryMonths;
            break;
        case YEARS_REG:
            *data = _years;
            break;
        default:
            return false;
    }

    return true;
}

bool MockPCF8563::readFromRegister(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data, size_t dataSize) {
    // error out if the read address is wrong
    if (slaveAddress != PCF8563_READ_ADDRESS) {
        return false;
    }

    // Read multiple bytes starting from the given registerAddress using readRegisterByte
    for (size_t i = 0; i < dataSize; i++) {
        if (!readRegisterByte(slaveAddress, registerAddress + i, &data[i])) {
            return false;
        }
    }

    return true;
}

bool MockPCF8563::writeRegisterByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t data) {
    // error out if the write address is wrong
    if (slaveAddress != PCF8563_WRITE_ADDRESS) {
        return false;
    }

    switch (registerAddress) {
        case CONTROL_STATUS1_REG:
            _controlStatus1 = data;
            break;
        case CONTROL_STATUS2_REG:
            _controlStatus2 = data;
            break;
        case SECONDS_REG:
            _seconds = data;
            break;
        case MINUTES_REG:
            _minutes = data;
            break;
        case HOURS_REG:
            _hours = data;
            break;
        case DAYS_REG:
            _days = data;
            break;
        case WEEKDAYS_REG:
            _weekdays = data;
            break;
        case CENTURY_MONTHS_REG:
            _centuryMonths = data;
            break;
        case YEARS_REG:
            _years = data;
            break;
        default:
            return false;
    }

    return true;
}

bool MockPCF8563::writeToRegister(uint8_t slaveAddress, uint8_t registerAddress, const uint8_t* data, size_t dataSize) {
    // error out if the write address is wrong
    if (slaveAddress != PCF8563_WRITE_ADDRESS) {
        return false;
    }

    // Write multiple bytes starting from the given registerAddress using writeRegisterByte
    for (size_t i = 0; i < dataSize; i++) {
        if (!writeRegisterByte(slaveAddress, registerAddress + i, data[i])) {
            return false;
        }
    }

    return true;
}