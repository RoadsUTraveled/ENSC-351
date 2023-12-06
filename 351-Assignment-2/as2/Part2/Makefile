CC = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror
LIBS = -lpthread -lm
TARGET = light_sampler
SRCS = A2D.c Joystick_Readings.c segDisplay.c common.c user_button_exit.c sampler.c sample_analysis.c main.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	cp $(TARGET) $(HOME)/cmpt433/public/myApps/

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean