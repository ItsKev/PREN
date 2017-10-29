################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/BitIoLdd4.c" \
"../Generated_Code/BitIoLdd5.c" \
"../Generated_Code/BitIoLdd6.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/I2C2.c" \
"../Generated_Code/LED1.c" \
"../Generated_Code/LED2.c" \
"../Generated_Code/LED3.c" \
"../Generated_Code/LEDpin4.c" \
"../Generated_Code/LEDpin5.c" \
"../Generated_Code/LEDpin6.c" \
"../Generated_Code/PE_LDD.c" \
"../Generated_Code/Vectors.c" \

C_SRCS += \
../Generated_Code/BitIoLdd4.c \
../Generated_Code/BitIoLdd5.c \
../Generated_Code/BitIoLdd6.c \
../Generated_Code/Cpu.c \
../Generated_Code/I2C2.c \
../Generated_Code/LED1.c \
../Generated_Code/LED2.c \
../Generated_Code/LED3.c \
../Generated_Code/LEDpin4.c \
../Generated_Code/LEDpin5.c \
../Generated_Code/LEDpin6.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/Vectors.c \

OBJS += \
./Generated_Code/BitIoLdd4.o \
./Generated_Code/BitIoLdd5.o \
./Generated_Code/BitIoLdd6.o \
./Generated_Code/Cpu.o \
./Generated_Code/I2C2.o \
./Generated_Code/LED1.o \
./Generated_Code/LED2.o \
./Generated_Code/LED3.o \
./Generated_Code/LEDpin4.o \
./Generated_Code/LEDpin5.o \
./Generated_Code/LEDpin6.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/Vectors.o \

OBJS_QUOTED += \
"./Generated_Code/BitIoLdd4.o" \
"./Generated_Code/BitIoLdd5.o" \
"./Generated_Code/BitIoLdd6.o" \
"./Generated_Code/Cpu.o" \
"./Generated_Code/I2C2.o" \
"./Generated_Code/LED1.o" \
"./Generated_Code/LED2.o" \
"./Generated_Code/LED3.o" \
"./Generated_Code/LEDpin4.o" \
"./Generated_Code/LEDpin5.o" \
"./Generated_Code/LEDpin6.o" \
"./Generated_Code/PE_LDD.o" \
"./Generated_Code/Vectors.o" \

C_DEPS += \
./Generated_Code/BitIoLdd4.d \
./Generated_Code/BitIoLdd5.d \
./Generated_Code/BitIoLdd6.d \
./Generated_Code/Cpu.d \
./Generated_Code/I2C2.d \
./Generated_Code/LED1.d \
./Generated_Code/LED2.d \
./Generated_Code/LED3.d \
./Generated_Code/LEDpin4.d \
./Generated_Code/LEDpin5.d \
./Generated_Code/LEDpin6.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/Vectors.d \

OBJS_OS_FORMAT += \
./Generated_Code/BitIoLdd4.o \
./Generated_Code/BitIoLdd5.o \
./Generated_Code/BitIoLdd6.o \
./Generated_Code/Cpu.o \
./Generated_Code/I2C2.o \
./Generated_Code/LED1.o \
./Generated_Code/LED2.o \
./Generated_Code/LED3.o \
./Generated_Code/LEDpin4.o \
./Generated_Code/LEDpin5.o \
./Generated_Code/LEDpin6.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/Vectors.o \

C_DEPS_QUOTED += \
"./Generated_Code/BitIoLdd4.d" \
"./Generated_Code/BitIoLdd5.d" \
"./Generated_Code/BitIoLdd6.d" \
"./Generated_Code/Cpu.d" \
"./Generated_Code/I2C2.d" \
"./Generated_Code/LED1.d" \
"./Generated_Code/LED2.d" \
"./Generated_Code/LED3.d" \
"./Generated_Code/LEDpin4.d" \
"./Generated_Code/LEDpin5.d" \
"./Generated_Code/LEDpin6.d" \
"./Generated_Code/PE_LDD.d" \
"./Generated_Code/Vectors.d" \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/BitIoLdd4.o: ../Generated_Code/BitIoLdd4.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/BitIoLdd4.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/BitIoLdd4.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/BitIoLdd5.o: ../Generated_Code/BitIoLdd5.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/BitIoLdd5.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/BitIoLdd5.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/BitIoLdd6.o: ../Generated_Code/BitIoLdd6.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/BitIoLdd6.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/BitIoLdd6.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Cpu.o: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/Cpu.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/Cpu.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/I2C2.o: ../Generated_Code/I2C2.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/I2C2.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/I2C2.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LED1.o: ../Generated_Code/LED1.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LED1.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LED1.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LED2.o: ../Generated_Code/LED2.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LED2.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LED2.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LED3.o: ../Generated_Code/LED3.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LED3.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LED3.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LEDpin4.o: ../Generated_Code/LEDpin4.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LEDpin4.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LEDpin4.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LEDpin5.o: ../Generated_Code/LEDpin5.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LEDpin5.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LEDpin5.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LEDpin6.o: ../Generated_Code/LEDpin6.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/LEDpin6.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/LEDpin6.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/PE_LDD.o: ../Generated_Code/PE_LDD.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/PE_LDD.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/PE_LDD.o"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Vectors.o: ../Generated_Code/Vectors.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Generated_Code/Vectors.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Generated_Code/Vectors.o"
	@echo 'Finished building: $<'
	@echo ' '


