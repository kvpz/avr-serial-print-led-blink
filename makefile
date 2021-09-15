CLOCK=16000000UL

# if the baud rate is not set correctly, avrdude will not be able to flash
BOOTLOADER_BAUD=115200 # Arduino bootloader default baud rate

AVR=/usr/lib/avr/include
PROGRAMMER_ID=arduino
PORT=/dev/ttyACM0

$(PRG).hex: $(PRG).elf
	avr-objcopy -O ihex -R .eeprom $(PRG).elf $(PRG).hex

$(PRG).elf: $(PRG).cpp
	avr-gcc -Os -DF_CPU=$(CLOCK) -mmcu=atmega328p -I$(AVR) -o $(PRG).elf $(PRG).cpp

upload: $(PRG).hex
	avrdude -F -V -c $(PROGRAMMER_ID) -p m328p -P $(PORT) -b $(BOOTLOADER_BAUD) -U flash:w:$(PRG).hex

clean:
	rm -f *.hex *.elf
