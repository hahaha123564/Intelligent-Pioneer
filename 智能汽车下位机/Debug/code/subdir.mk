################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Encoder.c \
../code/Flash.c \
../code/Gpio.c \
../code/Icar.c \
../code/Motor.c \
../code/Pid.c \
../code/Pit.c \
../code/Servo.c \
../code/UART.c 

COMPILED_SRCS += \
./code/Encoder.src \
./code/Flash.src \
./code/Gpio.src \
./code/Icar.src \
./code/Motor.src \
./code/Pid.src \
./code/Pit.src \
./code/Servo.src \
./code/UART.src 

C_DEPS += \
./code/Encoder.d \
./code/Flash.d \
./code/Gpio.d \
./code/Icar.d \
./code/Motor.d \
./code/Pid.d \
./code/Pit.d \
./code/Servo.d \
./code/UART.d 

OBJS += \
./code/Encoder.o \
./code/Flash.o \
./code/Gpio.o \
./code/Icar.o \
./code/Motor.o \
./code/Pid.o \
./code/Pit.o \
./code/Servo.o \
./code/UART.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fC:/Users/Դ/AURIX-v1.9.4-workspace/E01_01_button_switch_demo/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/Encoder.d ./code/Encoder.o ./code/Encoder.src ./code/Flash.d ./code/Flash.o ./code/Flash.src ./code/Gpio.d ./code/Gpio.o ./code/Gpio.src ./code/Icar.d ./code/Icar.o ./code/Icar.src ./code/Motor.d ./code/Motor.o ./code/Motor.src ./code/Pid.d ./code/Pid.o ./code/Pid.src ./code/Pit.d ./code/Pit.o ./code/Pit.src ./code/Servo.d ./code/Servo.o ./code/Servo.src ./code/UART.d ./code/UART.o ./code/UART.src

.PHONY: clean-code

