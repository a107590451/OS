obj-m += pro2.o

all:
	make -C /lib/modules/5.8.0-48-generic/build M=$(PWD) modules

clean:
	make -C /lib/modules/5.8.0-48-generic/build M=$(PWD) clean