a1upgpio : main.o gpio_lib.o \
	cc -o a1upgpio main.o 

main.o : main.c \
	cc -c gpio_lib.c

clean :
	rm a1upgpio main.o gpio_lib.o