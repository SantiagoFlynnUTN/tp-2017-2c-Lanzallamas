################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RGMaster.c \
../chat.c \
../cliente.c \
../conexionesYAMA.c \
../enviarArchivo.c \
../mainMaster.c \
../master.c \
../reduccionMaster.c \
../transformacionMaster.c 

OBJS += \
./RGMaster.o \
./chat.o \
./cliente.o \
./conexionesYAMA.o \
./enviarArchivo.o \
./mainMaster.o \
./master.o \
./reduccionMaster.o \
./transformacionMaster.o 

C_DEPS += \
./RGMaster.d \
./chat.d \
./cliente.d \
./conexionesYAMA.d \
./enviarArchivo.d \
./mainMaster.d \
./master.d \
./reduccionMaster.d \
./transformacionMaster.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -Icommons -Ipthread -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


