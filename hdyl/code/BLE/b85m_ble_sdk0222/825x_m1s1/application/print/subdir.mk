################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../application/print/putchar.c \
../application/print/u_printf.c 

OBJS += \
./application/print/putchar.o \
./application/print/u_printf.o 


# Each subdirectory must supply rules for building sources it contributes
application/print/%.o: ../application/print/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"C:\Users\86152\Desktop\b85m_ble_sdk0222" -I"C:\Users\86152\Desktop\b85m_ble_sdk0222\vendor\common" -I"C:\Users\86152\Desktop\b85m_ble_sdk0222\drivers\8258" -I"/b85m_ble_sdk\vendor\b85m_m1s1" -D__PROJECT_825x_M1S1__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


