################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../chat.c \
../cliente.c \
../conexionesDataNode.c \
../dataNode.c \
../mainDN.c \
../operacionesDataNode.c \
../serializacionDataNode.c 

OBJS += \
./chat.o \
./cliente.o \
./conexionesDataNode.o \
./dataNode.o \
./mainDN.o \
./operacionesDataNode.o \
./serializacionDataNode.o 

C_DEPS += \
./chat.d \
./cliente.d \
./conexionesDataNode.d \
./dataNode.d \
./mainDN.d \
./operacionesDataNode.d \
./serializacionDataNode.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


