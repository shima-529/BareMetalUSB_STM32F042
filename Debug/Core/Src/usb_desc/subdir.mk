################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/usb_desc/usb_desc.c 

OBJS += \
./Core/Src/usb_desc/usb_desc.o 

C_DEPS += \
./Core/Src/usb_desc/usb_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/usb_desc/usb_desc.o: ../Core/Src/usb_desc/usb_desc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F042x6 -DDEBUG -c -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -Os -ffunction-sections -fdata-sections -Wall -pedantic -fstack-usage -MMD -MP -MF"Core/Src/usb_desc/usb_desc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

