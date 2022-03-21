################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/8258/adc.c \
../drivers/8258/aes.c \
../drivers/8258/analog.c \
../drivers/8258/audio.c \
../drivers/8258/bsp.c \
../drivers/8258/clock.c \
../drivers/8258/emi.c \
../drivers/8258/flash.c \
../drivers/8258/gpio_8258.c \
../drivers/8258/i2c.c \
../drivers/8258/lpc.c \
../drivers/8258/qdec.c \
../drivers/8258/reboot.c \
../drivers/8258/rf_pa.c \
../drivers/8258/s7816.c \
../drivers/8258/spi.c \
../drivers/8258/timer.c \
../drivers/8258/uart.c \
../drivers/8258/watchdog.c 

OBJS += \
./drivers/8258/adc.o \
./drivers/8258/aes.o \
./drivers/8258/analog.o \
./drivers/8258/audio.o \
./drivers/8258/bsp.o \
./drivers/8258/clock.o \
./drivers/8258/emi.o \
./drivers/8258/flash.o \
./drivers/8258/gpio_8258.o \
./drivers/8258/i2c.o \
./drivers/8258/lpc.o \
./drivers/8258/qdec.o \
./drivers/8258/reboot.o \
./drivers/8258/rf_pa.o \
./drivers/8258/s7816.o \
./drivers/8258/spi.o \
./drivers/8258/timer.o \
./drivers/8258/uart.o \
./drivers/8258/watchdog.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/8258/%.o: ../drivers/8258/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314" -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314\vendor\common" -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314\drivers\8258" -I"/b85m_ble_sdk\vendor\b85m_m1s1" -D__PROJECT_825x_M1S1__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


