STLINK_VID_PID := 0483:374b
BAUD := 115200

build:
	arm-none-eabi-gcc main.c startup.c lib/uart.c lib/i2c.c lib/ssd1306.c -T linker_script.ld -o blink.elf -mcpu=cortex-m7 -mthumb -nostdlib

flash:
	openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c "program blink.elf verify reset exit"

dump: 
	arm-none-eabi-objdump -h blink.elf
	arm-none-eabi-nm blink.elf
	arm-none-eabi-objdump -s -j .isr_vector blink.elf

# Find the ST-LINK's virtual COM port and open it in tio.
log:
	@lsusb -d $(STLINK_VID_PID) >/dev/null 2>&1 || { \
	  echo "No ST-LINK on USB (looked for $(STLINK_VID_PID)). Board unplugged?"; \
	  exit 1; \
	}
	@PORT=$$(ls /dev/serial/by-id/*STLink* 2>/dev/null | head -n1); \
	if [ -z "$$PORT" ]; then \
	  echo "ST-LINK is on USB but exposes no serial port."; \
	  echo "Check that cdc_acm bound to it: dmesg | tail"; \
	  exit 1; \
	fi; \
	echo "STM32 VCP: $$PORT @ $(BAUD)"; \
	echo "Ctrl-T q to quit, Ctrl-T ? for help, Ctrl-T e for local echo"; \
	exec tio -b $(BAUD) -m INLCRNL "$$PORT"

.PHONY: build flash dump log
