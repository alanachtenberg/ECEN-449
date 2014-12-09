obj-m += audio_test.o
audio_test-objs := xac97.o audio_buffer.o

all:
	make -C ~/Lab5/linux-2.6.35.7 M=$(PWD) modules

clean:
	make -C /homes/ugrad/alanachtenberg/Lab5/linux-2.6.35.7 M=$(PWD) clean
