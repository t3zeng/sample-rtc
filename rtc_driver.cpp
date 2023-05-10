/**
 * rtc_driver.cpp - Implementation of the RTCDriver class.
 * The RTCDriver class provides a high-level interface to the PCF8563 real-time clock.
 * It depends on a separate low-level I2C driver implementation (i2c_driver.h).
 */

#include "rtc_driver.h"
#include "i2c_driver.h" // Assume a separate low-level I2C driver implementation

// define your registers here
#define PCF8563_SLAVE_READ_ADDRESS 0xa3
#define PCF8563_SLAVE_WRITE_ADDRESS 0xa2
#define CONTROL_STATUS1_REG 0x00
#define CONTROL_STATUS2_REG 0x01
#define SECONDS_REG 0x02
#define MINUTES_REG 0x03
#define HOURS_REG 0x04
#define DAYS_REG 0x05
#define WEEKDAYS_REG 0x06 // unused
#define CENTURY_MONTHS_REG 0x07
#define YEARS_REG 0x08

// helper function to convert BCD values to binary
static uint8_t bcdToBinary(uint8_t bcdValue) {
    return ((bcdValue >> 4) * 10) + (bcdValue & 0x0F);
}

// helper function to convert decimal values to BCD
static uint8_t bcdFromDecimal(uint8_t decimalValue) {
    return ((decimalValue / 10) << 4) | (decimalValue % 10);
}

// helper function to calculate weekday
static uint8_t calculateWeekday(uint8_t day, uint8_t month, uint16_t year) {
    // January and February are counted as months 13 and 14 of the previous year
    if (month <= 2) {
        month += 12;
        year -= 1;
    }
    
    // Calculate the century (first two digits of the year) and year within century
    uint16_t century = year / 100;
    uint16_t yearWithinCentury = year % 100;
    
    // Calculate the weekday using Zeller's congruence algorithm
    uint8_t weekday = (day + (13 * (month + 1) / 5) + yearWithinCentury + (yearWithinCentury / 4) + (century / 4) - (2 * century)) % 7;
    
    // Convert the result to match the PCF8563's weekday encoding (0 = Sunday, 1 = Monday, etc.)
    weekday += 6;
    weekday %= 7;
    
    return weekday;
}

RTCDriver::RTCDriver() {
    // Constructor
}

bool RTCDriver::initialize() {
    uint8_t cs1 = 0; // Normal operation PCF8563 Rev.11 [8.3.1]
    uint8_t cs2 = 0; // Not needed for get/set time operation PCF8563 Rev.11 [8.3.2]

    // Initialize the I2C driver
    if (!I2CDriver::initialize()) {
        return false;
    }

    // Additional RTC-specific initialization steps can be performed here
    if (!I2CDriver::writeToRegister(PCF8563_SLAVE_WRITE_ADDRESS, CONTROL_STATUS1_REG, &cs1, sizeof(cs1))) {
        return false;
    }
    
    if (!I2CDriver::writeToRegister(PCF8563_SLAVE_WRITE_ADDRESS, CONTROL_STATUS2_REG, &cs2, sizeof(cs2))) {
        return false;
    }

    return true;
}

/**
 * Read the current time from the RTC, the hours are given in 24h format
 * No special wrap-around cases need to be handled due to the following feature listed in the datasheet
 * PCF8563 Rev.11 [8.5]
 * During read/write operations, the time counting circuits (memory locations 02h through 08h) are blocked.
 * This prevents
 * • Faulty reading of the clock and calendar during a carry condition
 * • Incrementing the time registers, during the read cycle
 * 
 * After the R/W is completed the time circuit is restored. The full R/W must be done in one second and as a consequence
 * reading or writing must be done in one go
 */ 
bool RTCDriver::getTime(uint8_t& hours, uint8_t& minutes, uint8_t& seconds) {
    uint8_t bcdTime[3]; // array to hold the bcd values for seconds, minutes, hours

    // read from register 02h to 08h in one go is recommended in PCF8563 Rev.11 [8.5]
    // technically can save stack space and a little bit of read time if we just read 3 bytes
    if (!I2CDriver::readFromRegister(PCF8563_SLAVE_READ_ADDRESS, SECONDS_REG, bcdTime, 3)) {
        return false;
    }

    // Convert the BCD values to binary
    seconds = bcdToBinary(bcdTime[0]);
    minutes = bcdToBinary(bcdTime[1]);
    hours = bcdToBinary(bcdTime[2]);

    return true;
}

bool RTCDriver::setDate(uint8_t day, uint8_t month, uint16_t year) {
    // Set the date on the RTC
    uint8_t year_short = year % 100; // Extract the year within the century
    uint8_t weekday = calculateWeekday(day, month, year); // extract weekday

    // Write the day, month, year within the century, and century flag to the RTC registers
    uint8_t data[4] = {0};
    data[0] = bcdFromDecimal(day); // ADDR: 0x05 -> set the day
    data[1] = bcdFromDecimal(weekday); // ADDR: 0x06 -> set the weekday
    data[2] = bcdFromDecimal(month) | (uint8_t)(year >= 2000) << 7; // ADDR: 0x07 -> set the centuries and month
    data[3] = bcdFromDecimal(year_short); // ADDR: 0x08 -> Set year within century

    if (!I2CDriver::writeToRegister(PCF8563_SLAVE_WRITE_ADDRESS, DAYS_REG, data, sizeof(data))) {
        return false;
    }

    return true;
}

