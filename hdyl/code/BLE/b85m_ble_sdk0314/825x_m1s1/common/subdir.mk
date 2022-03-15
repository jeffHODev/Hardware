################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/string.c \
../common/utility.c 

OBJS += \
./common/string.o \
./common/utility.o 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314" -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314\vendor\common" -I"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0314\drivers\8258" -I"/b85m_ble_sdk\vendor\b85m_m1s1" -D__PROJECT_825x_M1S1__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


