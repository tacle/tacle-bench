################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SourceFiles/controlexclusion.c \
../SourceFiles/debounce.c \
../SourceFiles/debounce_passenger_Front.c \
../SourceFiles/powerwindow_control.c \
../SourceFiles/powerwindow_control_data.c 

OBJS += \
./SourceFiles/controlexclusion.o \
./SourceFiles/debounce.o \
./SourceFiles/debounce_passenger_Front.o \
./SourceFiles/powerwindow_control.o \
./SourceFiles/powerwindow_control_data.o 

C_DEPS += \
./SourceFiles/controlexclusion.d \
./SourceFiles/debounce.d \
./SourceFiles/debounce_passenger_Front.d \
./SourceFiles/powerwindow_control.d \
./SourceFiles/powerwindow_control_data.d 


# Each subdirectory must supply rules for building sources it contributes
SourceFiles/%.o: ../SourceFiles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


