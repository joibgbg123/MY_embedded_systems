obj-m+=mydev_joi.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	g++ -o main -lpthread main.cpp
	sudo insmod mydev_joi.ko
	sudo mknod -m 666 /dev/mydev_joi c 240 0
clean:
	rm -f main
	sudo rm /dev/mydev_joi
	sudo rmmod mydev_joi
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean