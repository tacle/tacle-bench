################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../powerwindow_SourceFiles/powerwindow_controlexclusion.c \
../powerwindow_SourceFiles/powerwindow_debounce.c \
../powerwindow_SourceFiles/powerwindow_debounce_passenger_Front.c \
../powerwindow_SourceFiles/powerwindow_powerwindow_control.c \
../powerwindow_SourceFiles/powerwindow_powerwindow_control_data.c 

OBJS += \
./powerwindow_SourceFiles/powerwindow_controlexclusion.o \
./powerwindow_SourceFiles/powerwindow_debounce.o \
./powerwindow_SourceFiles/powerwindow_debounce_passenger_Front.o \
./powerwindow_SourceFiles/powerwindow_powerwindow_control.o \
./powerwindow_SourceFiles/powerwindow_powerwindow_control_data.o 

C_DEPS += \
./powerwindow_SourceFiles/powerwindow_controlexclusion.d \
./powerwindow_SourceFiles/powerwindow_debounce.d \
./powerwindow_SourceFiles/powerwindow_debounce_passenger_Front.d \
./powerwindow_SourceFiles/powerwindow_powerwindow_control.d \
./powerwindow_SourceFiles/powerwindow_powerwindow_control_data.d 


# Each subdirectory must supply rules for building sources it contributes
powerwindow_SourceFiles/%.o: ../powerwindow_SourceFiles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


