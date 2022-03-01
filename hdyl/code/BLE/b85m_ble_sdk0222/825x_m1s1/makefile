################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/common/subdir.mk
-include vendor/b85m_m1s1/subdir.mk
-include drivers/8258/subdir.mk
-include drivers/8258/flash/subdir.mk
-include drivers/8258/driver_ext/subdir.mk
-include common/subdir.mk
-include boot/8278/subdir.mk
-include boot/8258/subdir.mk
-include boot/8253/subdir.mk
-include application/usbstd/subdir.mk
-include application/print/subdir.mk
-include application/keyboard/subdir.mk
-include application/app/subdir.mk
-include algorithm/lc3/src/fft_lib/kiss_fft_custom/subdir.mk
-include algorithm/lc3/src/fft_lib/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
825x_m1s1.lst \

FLASH_IMAGE += \

SIZEDUMMY += \
sizedummy \


# All Target
all: b85m_ble_sdk0222.elf secondary-outputs

# Tool invocations
b85m_ble_sdk0222.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\proj_lib" -T ../boot.link -o"b85m_ble_sdk0222.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

825x_m1s1.lst: b85m_ble_sdk0222.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S b85m_ble_sdk0222.elf  >"825x_m1s1.lst"
	@echo 'Finished building: $@'
	@echo ' '

: b85m_ble_sdk0222.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy b85m_ble_sdk0222.elf
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: b85m_ble_sdk0222.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t b85m_ble_sdk0222.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) b85m_ble_sdk0222.elf
	-@echo ' '

post-build:
	-"D:\work\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222/tl_check_fw.sh"   825x_m1s1   b85m_ble_sdk0222
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
