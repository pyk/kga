# Configs
CFLAGS=-Wall -std=c99 -Wpedantic

# Build the binary
.PHONY: kga
kga: src/main.c src/knapsack.c
	$(CC) $(CFLAGS) src/main.c src/knapsack.c -o bin/kga

# Test
.PHONY: test
test: src/knapsack.h src/knapsack.c src/knapsack_test.c
	$(CC) $(CFLAGS) -g src/knapsack_test.c src/knapsack.c -o bin/kga-test
	valgrind -q --track-origins=yes --leak-check=yes ./bin/kga-test
