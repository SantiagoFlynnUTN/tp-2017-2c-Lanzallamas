################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../chat.c \
../conexionesFileSystem.c \
../configuracionFileSystem.c \
../envioBloques.c \
../estructurasFileSystem.c \
../fileSystem.c \
../funcionesConsolaFS.c \
../inicializacionFileSystem.c \
../mainFS.c \
../recepcionBloques.c \
../serializacionFileSystem.c \
../servidor.c \
../utilidadesFileSystem.c 

OBJS += \
./chat.o \
./conexionesFileSystem.o \
./configuracionFileSystem.o \
./envioBloques.o \
./estructurasFileSystem.o \
./fileSystem.o \
./funcionesConsolaFS.o \
./inicializacionFileSystem.o \
./mainFS.o \
./recepcionBloques.o \
./serializacionFileSystem.o \
./servidor.o \
./utilidadesFileSystem.o 

C_DEPS += \
./chat.d \
./conexionesFileSystem.d \
./configuracionFileSystem.d \
./envioBloques.d \
./estructurasFileSystem.d \
./fileSystem.d \
./funcionesConsolaFS.d \
./inicializacionFileSystem.d \
./mainFS.d \
./recepcionBloques.d \
./serializacionFileSystem.d \
./servidor.d \
./utilidadesFileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-2c-Lanzallamas/Z-Commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


