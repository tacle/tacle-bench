################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../powerwindow.c \
../powerwindow_controlexclusion.c \
../powerwindow_debounce.c \
../powerwindow_debounce_passenger_Front.c \
../powerwindow_powerwindow_control.c \
../powerwindow_powerwindow_control_data.c 

OBJS += \
./powerwindow.o \
./powerwindow_controlexclusion.o \
./powerwindow_debounce.o \
./powerwindow_debounce_passenger_Front.o \
./powerwindow_powerwindow_control.o \
./powerwindow_powerwindow_control_data.o 

C_DEPS += \
./powerwindow.d \
./powerwindow_controlexclusion.d \
./powerwindow_debounce.d \
./powerwindow_debounce_passenger_Front.d \
./powerwindow_powerwindow_control.d \
./powerwindow_powerwindow_control_data.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


