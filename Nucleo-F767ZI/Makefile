build:
	arm-none-eabi-gcc main.c startup.c -T linker_script.ld -o blink.elf -mcpu=cortex-m7 -mthumb -nostdlib

flash:
	openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c "program blink.elf verify reset exit"

dump: 
	arm-none-eabi-objdump -h blink.elf
	arm-none-eabi-nm blink.elf
	arm-none-eabi-objdump -s -j .isr_vector blink.elf

