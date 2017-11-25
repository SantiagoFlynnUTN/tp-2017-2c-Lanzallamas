################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RGWorker.c \
../apareo.c \
../chat.c \
../cliente.c \
../enviarArchivo.c \
../mainApareo.c \
../mainWorker.c \
../reduccionWorker.c \
../serializacionWorker.c \
../servidor.c \
../transformacionWorker.c 

OBJS += \
./RGWorker.o \
./apareo.o \
./chat.o \
./cliente.o \
./enviarArchivo.o \
./mainApareo.o \
./mainWorker.o \
./reduccionWorker.o \
./serializacionWorker.o \
./servidor.o \
./transformacionWorker.o 

C_DEPS += \
./RGWorker.d \
./apareo.d \
./chat.d \
./cliente.d \
./enviarArchivo.d \
./mainApareo.d \
./mainWorker.d \
./reduccionWorker.d \
./serializacionWorker.d \
./servidor.d \
./transformacionWorker.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


