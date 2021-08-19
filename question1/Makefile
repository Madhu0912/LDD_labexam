obj-m:=ques1.o
KDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o read_app read_app.c
	gcc -o write_app write_app.c

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf write_app read_app
