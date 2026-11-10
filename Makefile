ifneq ($(KERNELRELEASE),)
obj-m := hello1.o hello2.o
ccflags-y := -I$(src)/inc
else
KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
endif
