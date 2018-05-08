################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Application.c" \
"../Sources/Electromagnet_Driver.c" \
"../Sources/Events.c" \
"../Sources/LED.c" \
"../Sources/Lockup_Table.c" \
"../Sources/MeasurmentHandler.c" \
"../Sources/Shell.c" \
"../Sources/Stepper.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/Application.c \
../Sources/Electromagnet_Driver.c \
../Sources/Events.c \
../Sources/LED.c \
../Sources/Lockup_Table.c \
../Sources/MeasurmentHandler.c \
../Sources/Shell.c \
../Sources/Stepper.c \
../Sources/main.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/Application.o \
./Sources/Electromagnet_Driver.o \
./Sources/Events.o \
./Sources/LED.o \
./Sources/Lockup_Table.o \
./Sources/MeasurmentHandler.o \
./Sources/Shell.o \
./Sources/Stepper.o \
./Sources/main.o \
./Sources/sa_mtb.o \

OBJS_QUOTED += \
"./Sources/Application.o" \
"./Sources/Electromagnet_Driver.o" \
"./Sources/Events.o" \
"./Sources/LED.o" \
"./Sources/Lockup_Table.o" \
"./Sources/MeasurmentHandler.o" \
"./Sources/Shell.o" \
"./Sources/Stepper.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \

C_DEPS += \
./Sources/Application.d \
./Sources/Electromagnet_Driver.d \
./Sources/Events.d \
./Sources/LED.d \
./Sources/Lockup_Table.d \
./Sources/MeasurmentHandler.d \
./Sources/Shell.d \
./Sources/Stepper.d \
./Sources/main.d \
./Sources/sa_mtb.d \

OBJS_OS_FORMAT += \
./Sources/Application.o \
./Sources/Electromagnet_Driver.o \
./Sources/Events.o \
./Sources/LED.o \
./Sources/Lockup_Table.o \
./Sources/MeasurmentHandler.o \
./Sources/Shell.o \
./Sources/Stepper.o \
./Sources/main.o \
./Sources/sa_mtb.o \

C_DEPS_QUOTED += \
"./Sources/Application.d" \
"./Sources/Electromagnet_Driver.d" \
"./Sources/Events.d" \
"./Sources/LED.d" \
"./Sources/Lockup_Table.d" \
"./Sources/MeasurmentHandler.d" \
"./Sources/Shell.d" \
"./Sources/Stepper.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Application.o: ../Sources/Application.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Application.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Application.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Electromagnet_Driver.o: ../Sources/Electromagnet_Driver.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Electromagnet_Driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Electromagnet_Driver.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Events.o: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Events.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Events.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/LED.o: ../Sources/LED.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/LED.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/LED.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Lockup_Table.o: ../Sources/Lockup_Table.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Lockup_Table.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Lockup_Table.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/MeasurmentHandler.o: ../Sources/MeasurmentHandler.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/MeasurmentHandler.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/MeasurmentHandler.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Shell.o: ../Sources/Shell.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Shell.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Shell.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Stepper.o: ../Sources/Stepper.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Stepper.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Stepper.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


