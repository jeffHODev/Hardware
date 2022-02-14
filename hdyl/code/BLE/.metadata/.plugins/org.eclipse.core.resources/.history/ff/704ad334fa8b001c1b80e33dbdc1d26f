################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../boot/8278/cstartup_8278.S \
../boot/8278/cstartup_8278_RET_32K.S 

OBJS += \
./boot/8278/cstartup_8278.o \
./boot/8278/cstartup_8278_RET_32K.o 


# Each subdirectory must supply rules for building sources it contributes
boot/8278/%.o: ../boot/8278/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -DMCU_STARTUP_8258 -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


