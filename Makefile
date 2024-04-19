.DEFAULT_GOAL := all # Sets default target, ie what runs when you type in make
obj-m += pad.o

instructions:
	echo "Run run1, then make, then run2"
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run1:
	gcc gtapp.c -o gtapp 'pkg-config --cflags --libs gtk+-3.0'
run2:
	sudo insmod pad.ko
	sudo mknod /dev/virtual_keypad c 510 0
test:
	sudo ./gtapp
	
