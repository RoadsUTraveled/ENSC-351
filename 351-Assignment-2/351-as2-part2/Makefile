all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror segDisplay.c common.c user_button_exit.c main.c -o display -lpthread
	cp display $(HOME)/cmpt433/public/myApps/
