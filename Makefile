CC = g++
CFLAGS = -Wall -Wextra -std=c++14
GTEST_DIR = googletest-main

# Update this variable if you installed Google Test in a non-standard location
GTEST_INCLUDE = -I$(GTEST_DIR)/googletest/include
GTEST_LIB = $(GTEST_DIR)/focal_build/lib/libgtest.a

SRC = rtc_driver.cpp i2c_driver.cpp mock_pcf8563.cpp
OBJ = $(SRC:.cpp=.o)

all: main test

main: main.o $(OBJ)
	$(CC) $(CFLAGS) -o main main.o $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $(GTEST_INCLUDE) -c $< -o $@

test: rtc_driver_test.o $(OBJ)
	$(CC) $(CFLAGS) $(GTEST_INCLUDE) -o test rtc_driver_test.o $(OBJ) -L$(GTEST_DIR)/focal_build/lib -lgtest -lgtest_main
	./test


clean:
	rm -f *.o main test
