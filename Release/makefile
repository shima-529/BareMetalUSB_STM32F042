################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Registers_F042.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Registers_F042.list \

OBJCOPY_BIN += \
Registers_F042.bin \


# All Target
all: Registers_F042.elf secondary-outputs

# Tool invocations
Registers_F042.elf: $(OBJS) $(USER_OBJS) /Users/yuki/Dropbox/STM32CubeIDE/workspace/Registers_F042/STM32F042K6TX_FLASH.ld
	arm-none-eabi-gcc -o "Registers_F042.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"/Users/yuki/Dropbox/STM32CubeIDE/workspace/Registers_F042/STM32F042K6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Registers_F042.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Registers_F042.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Registers_F042.list"
	@echo 'Finished building: $@'
	@echo ' '

Registers_F042.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Registers_F042.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
