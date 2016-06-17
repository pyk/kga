# Configs
CC=gcc
CFLAGS=-Wall -std=c99 -Wpedantic

# Build the binary
.PHONY: kga
kga: src/main.c
	$(CC) $(CFLAGS) src/main.c -o bin/kga
