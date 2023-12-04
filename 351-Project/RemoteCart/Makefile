all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror common.c CartMove.c main.c -o carttest -lpthread -lm
	cp carttest $(HOME)/cmpt433/public/myApps/

.PHONY: all
