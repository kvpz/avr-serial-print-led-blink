CLOCK=16000000UL
BAUD=115200
#BAUD=57600
#BAUD=9600

AVR=/usr/lib/avr/include

$(PRG).hex: $(PRG).elf
	avr-objcopy -O ihex -R .eeprom $(PRG).elf $(PRG).hex

$(PRG).elf: $(PRG).cpp
	avr-gcc -Os -DF_CPU=$(CLOCK) -mmcu=atmega328p -I$(AVR) -o $(PRG).x $(PRG).cpp

upload: $(PRG).hex
	avrdude -F -V -c arduino -p m328p -P /dev/ttyACM0 -b $(BAUD) -U flash:w:$(PRG).hex

clean:
	rm -f *.hex *.elf
