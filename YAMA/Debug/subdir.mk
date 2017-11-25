################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RGYAMA.c \
../chat.c \
../clienteFS.c \
../conexiones.c \
../mainYAMA.c \
../planificacionYama.c \
../respuestaTransformacion.c \
../serializacion.c \
../servidor.c \
../yama.c 

OBJS += \
./RGYAMA.o \
./chat.o \
./clienteFS.o \
./conexiones.o \
./mainYAMA.o \
./planificacionYama.o \
./respuestaTransformacion.o \
./serializacion.o \
./servidor.o \
./yama.o 

C_DEPS += \
./RGYAMA.d \
./chat.d \
./clienteFS.d \
./conexiones.d \
./mainYAMA.d \
./planificacionYama.d \
./respuestaTransformacion.d \
./serializacion.d \
./servidor.d \
./yama.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


