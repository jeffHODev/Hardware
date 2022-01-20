################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../algorithm/lc3/src/fft_lib/kiss_fft.c 

OBJS += \
./algorithm/lc3/src/fft_lib/kiss_fft.o 


# Each subdirectory must supply rules for building sources it contributes
algorithm/lc3/src/fft_lib/%.o: ../algorithm/lc3/src/fft_lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\86152\Desktop\b85m_ble_sdk" -I"C:\Users\86152\Desktop\b85m_ble_sdk\vendor\common" -I"C:\Users\86152\Desktop\b85m_ble_sdk\drivers\8258" -D__PROJECT_825x_M1S1__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


