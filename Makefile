# Configs
CFLAGS=-Wall -std=c99 -Wpedantic

# Build the binary
.PHONY: kga
kga:
	$(CC) $(CFLAGS) src/main.c src/knapsack.c src/matrix.c src/vector.c -o bin/kga

# Test
.PHONY: test
test:
	@echo "Test: knapsack"
	$(CC) $(CFLAGS) -g src/knapsack_test.c src/knapsack.c -o bin/knapsack-test
	valgrind -q --track-origins=yes --leak-check=yes ./bin/knapsack-test
	@echo "Test: matrix"
	$(CC) $(CFLAGS) -g src/matrix_test.c src/matrix.c -o bin/matrix-test
	valgrind -q --track-origins=yes --leak-check=yes ./bin/matrix-test
	@echo "Test: vector"
	$(CC) $(CFLAGS) -g src/vector_test.c src/vector.c -o bin/vector-test
	valgrind -q --track-origins=yes --leak-check=yes ./bin/vector-test
