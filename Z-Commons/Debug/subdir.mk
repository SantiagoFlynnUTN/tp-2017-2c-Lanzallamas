################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archivos.c \
../cosas.c \
../protocoloComunicacion.c \
../prueba.c \
../sockets.c \
../structUtiles.c 

OBJS += \
./archivos.o \
./cosas.o \
./protocoloComunicacion.o \
./prueba.o \
./sockets.o \
./structUtiles.o 

C_DEPS += \
./archivos.d \
./cosas.d \
./protocoloComunicacion.d \
./prueba.d \
./sockets.d \
./structUtiles.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


