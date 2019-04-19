################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf
MAKELIBDIR := ../../Test_sd_bsp/


# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
Test_sd.elf.size \


# All Target
all: pre-build libr main-build

# Main-build Target
main-build: Test_sd.elf secondary-outputs

# Tool invocations
Test_sd.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM v7 gcc linker'
	arm-none-eabi-gcc -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs=Xilinx.spec -Wl,-T -Wl,../src/lscript.ld -L../../Test_sd_bsp/ps7_cortexa9_0/lib -o "Test_sd.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Test_sd.elf.size: Test_sd.elf
	@echo 'Invoking: ARM v7 Print Size'
	arm-none-eabi-size Test_sd.elf  |tee "Test_sd.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

libr:
	echo 'Building libraries'
	$(MAKE) -C $(MAKELIBDIR)
# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) Test_sd.elf
	-@echo ' '

pre-build:
	-a9-linaro-pre-build-step
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents pre-build
.SECONDARY: main-build pre-build

-include ../makefile.targets
