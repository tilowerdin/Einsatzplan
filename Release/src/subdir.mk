################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Constants.cpp \
../src/Conversion.cpp \
../src/Structures.cpp \
../src/SwimPlan.cpp 

OBJS += \
./src/Constants.o \
./src/Conversion.o \
./src/Structures.o \
./src/SwimPlan.o 

CPP_DEPS += \
./src/Constants.d \
./src/Conversion.d \
./src/Structures.d \
./src/SwimPlan.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


