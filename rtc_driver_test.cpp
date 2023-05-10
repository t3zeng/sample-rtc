#include "gtest/gtest.h"
#include "rtc_driver.h"

class RTCDriverTest : public ::testing::Test {
protected:
    RTCDriver rtc;

    virtual void SetUp() {
        ASSERT_TRUE(rtc.initialize());
    }

    virtual void TearDown() {
        // Perform any necessary cleanup
    }
};

TEST_F(RTCDriverTest, SetDateTest) {
    uint8_t day = 12;
    uint8_t month = 5;
    uint16_t year = 2023;

    EXPECT_TRUE(rtc.setDate(day, month, year));
}

TEST_F(RTCDriverTest, GetTimeTest) {
    // Test getting the current time
    uint8_t hours, minutes, seconds;

    EXPECT_TRUE(rtc.getTime(hours, minutes, seconds));

    // Add additional expectations based on the current time
    EXPECT_LE(hours, 23);
    EXPECT_GE(hours, 0);
    EXPECT_LE(minutes, 59);
    EXPECT_GE(minutes, 0);
    EXPECT_LE(seconds, 59);
    EXPECT_GE(seconds, 0);
}


TEST_F(RTCDriverTest, SetDateBoundaryTest) {
    // Test setting the date to the earliest and latest possible values
    uint8_t day = 1;
    uint8_t month = 1;
    uint16_t year = 1900;

    EXPECT_TRUE(rtc.setDate(day, month, year));

    day = 31;
    month = 12;
    year = 2099;

    EXPECT_TRUE(rtc.setDate(day, month, year));
}