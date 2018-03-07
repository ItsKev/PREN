################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/startup.c" \

C_SRCS += \
../Project_Settings/Startup_Code/startup.c \

OBJS += \
./Project_Settings/Startup_Code/startup.o \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/startup.o" \

C_DEPS += \
./Project_Settings/Startup_Code/startup.d \

C_DEPS_QUOTED += \
"./Project_Settings/Startup_Code/startup.d" \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/startup.o \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/startup.o: ../Project_Settings/Startup_Code/startup.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C++ Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-g++" "$<" @"Project_Settings/Startup_Code/startup.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Project_Settings/Startup_Code/startup.o"
	@echo 'Finished building: $<'
	@echo ' '


