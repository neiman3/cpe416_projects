SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BOARD_LIB = $(SELF_DIR)/library
SOURCE_LIB = $(wildcard $(BOARD_LIB)/*.c)
CLOCK_RATE = 16000000L
#edit the device to match the device on your computer
USB_ARR = $(shell ls /dev | grep cu.usb)
MAC_DEVICE = `ls /dev/$(USB_ARR)`
LAB = lab3

local: lab3/test.c
	cc lab3/test.c -o main.o lab3/neuralnet.c lab3/proportional.c

main: main.c $(wildcard $(BOARD_LIB)/*.c)
	/opt/homebrew/Cellar/avr-gcc@10/10.3.0_2/bin/avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf main.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

p1: $(SELF_DIR)/$(LAB)/$(LAB)_part1.c $(wildcard $(BOARD_LIB)/*.c)
	/opt/homebrew/Cellar/avr-gcc@10/10.3.0_2/bin/avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf $(SELF_DIR)/$(LAB)/$(LAB)_part1.c $(wildcard $(BOARD_LIB)/*.c) ./$(LAB)/proportional.c
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex


p2: $(SELF_DIR)/$(LAB)/$(LAB)_part2.c $(wildcard $(BOARD_LIB)/*.c)
	/opt/homebrew/Cellar/avr-gcc@10/10.3.0_2/bin/avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf $(SELF_DIR)/$(LAB)/$(LAB)_part2.c $(wildcard $(BOARD_LIB)/*.c) ./$(LAB)/progressbar.c ./$(LAB)/proportional.c ./$(LAB)/neuralnet.c
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex


p3: $(SELF_DIR)/$(LAB)/$(LAB)_part3.c $(wildcard $(BOARD_LIB)/*.c)
	/opt/homebrew/Cellar/avr-gcc@10/10.3.0_2/bin/avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf $(SELF_DIR)/$(LAB)/$(LAB)_part3.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex


p4: $(SELF_DIR)/$(LAB)/$(LAB)_part4.c $(wildcard $(BOARD_LIB)/*.c)
	/opt/homebrew/Cellar/avr-gcc@10/10.3.0_2/bin/avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -o main.elf $(SELF_DIR)/$(LAB)/$(LAB)_part4.c $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex

lib: $(wildcard $(BOARD_LIB)/*.c)
	$(foreach var,$(SOURCE_LIB), \
        avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -O2 -c -o $(var:.c=.o) $(var); )

program_windows: 
	avrdude -F -pm645 -Pcom4 -carduino -u -U flash:w:main.hex


flash: main.elf
	avrdude -pm645 -P $(MAC_DEVICE) -c arduino -F -u -U flash:w:main.hex

program_linux: main.elf
	avrdude -pm645 -P /dev/bumblebee -c arduino -F -u -U flash:w:main.hex
flashvm: main.elf
	avrdude -pm645 -P /dev/ttyACM0 -c arduino -F -u -U flash:w:main.hex
clean:
	rm -fr *.elf *.hex *.o
