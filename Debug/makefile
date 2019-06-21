################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/simha/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./startup_msp432p401r_ccs.obj" \
"./system_msp432p401r.obj" \
"./inc/Clock.obj" \
"./inc/CortexM.obj" \
"./inc/EUSCIA0.obj" \
"./inc/FIFO0.obj" \
"./inc/Motor.obj" \
"./inc/PWM.obj" \
"./inc/TimerA0.obj" \
"./inc/TimerA1.obj" \
"./inc/TimerA2.obj" \
"./inc/UART0.obj" \
"./inc/UART1.obj" \
"../msp432p401r.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include inc/subdir_vars.mk
-include subdir_rules.mk
-include inc/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
RSLK-Triangulation.out \

EXE_OUTPUTS__QUOTED += \
"RSLK-Triangulation.out" \

BIN_OUTPUTS += \
RSLK-Triangulation.hex \

BIN_OUTPUTS__QUOTED += \
"RSLK-Triangulation.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "RSLK-Triangulation.out"

# Tool invocations
RSLK-Triangulation.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/home/simha/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"RSLK-Triangulation.map" --heap_size=1024 --stack_size=512 -i"/home/simha/ti/ccs901/ccs/ccs_base/arm/include" -i"/home/simha/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/lib" -i"/home/simha/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="RSLK-Triangulation_linkInfo.xml" --rom_model -o "RSLK-Triangulation.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

RSLK-Triangulation.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/home/simha/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armhex" --memwidth=8 --romwidth=8 -o "RSLK-Triangulation.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "startup_msp432p401r_ccs.obj" "system_msp432p401r.obj" "inc/Clock.obj" "inc/CortexM.obj" "inc/EUSCIA0.obj" "inc/FIFO0.obj" "inc/Motor.obj" "inc/PWM.obj" "inc/TimerA0.obj" "inc/TimerA1.obj" "inc/TimerA2.obj" "inc/UART0.obj" "inc/UART1.obj" 
	-$(RM) "main.d" "startup_msp432p401r_ccs.d" "system_msp432p401r.d" "inc/Clock.d" "inc/CortexM.d" "inc/EUSCIA0.d" "inc/FIFO0.d" "inc/Motor.d" "inc/PWM.d" "inc/TimerA0.d" "inc/TimerA1.d" "inc/TimerA2.d" "inc/UART0.d" "inc/UART1.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

