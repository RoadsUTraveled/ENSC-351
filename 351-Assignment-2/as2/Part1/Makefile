# Makefile to cross-compile 'noworky' program

# Cross-compiler settings
CC = arm-linux-gnueabi-gcc # Replace with your cross-compiler
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -Wshadow

# Target binary
TARGET = noworky

# Source files
SOURCES = noworky.c # Add or update source files
DESTINATION = $(HOME)/cmpt433/public/myApps/
# Object files
OBJECTS = $(SOURCES:.c=.o)

# Build rule
all: $(TARGET)
		cp $(TARGET) $(DESTINATION)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
