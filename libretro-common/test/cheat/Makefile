# Makefile for cheat auto-load unit tests
# Uses Check testing framework like other RetroArch tests

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I../../include
LDFLAGS = -lcheck -lm -lpthread -lrt

TARGET = test_cheat_auto_load
SOURCES = test_cheat_auto_load.c

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

install-deps:
	# Install Check testing framework (Ubuntu/Debian)
	sudo apt-get install check libcheck-dev

help:
	@echo "Available targets:"
	@echo "  all         - Build test executable"
	@echo "  test        - Run tests"  
	@echo "  clean       - Clean build files"
	@echo "  install-deps - Install testing dependencies"
	@echo "  help        - Show this help"