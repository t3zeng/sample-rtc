#include "i2c_driver.h"
#include "mock_pcf8563.h"

MockPCF8563 pcfMock;  // create a single instance of the mock PCF8563

bool I2CDriver::initialize() {
    // do any necessary initialization, this is based on hardware running this code and what it takes to initialize I2C
    
    // use mock rtc initialize
    pcfMock.initialize();
    return true;
}

bool I2CDriver::readFromRegister(uint8_t slaveAddress, uint8_t registerAddress, uint8_t* data, size_t dataSize) {
    // hardware specific implementation to perform I2C read would go here

    // use mock rtc as if reading from a real hw pcf8563
    return pcfMock.readFromRegister(slaveAddress, registerAddress, data, dataSize);
}

bool I2CDriver::writeToRegister(uint8_t slaveAddress, uint8_t registerAddress, const uint8_t* data, size_t dataSize) {
    // hardware specific implementation to perform I2C write would go here

    // use mock rtc as if writing to a real hw pcf8563
    return pcfMock.writeToRegister(slaveAddress, registerAddress, data, dataSize);
}
