#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stddef.h>
#include <stdint.h>

class I2CDriver {
public:
    static bool initialize();
    static bool readFromRegister(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data, size_t dataSize);
    static bool writeToRegister(uint8_t slaveAddress, uint8_t registerAddress, const uint8_t* data, size_t dataSize);
};

#endif
