CC = gcc
CFLAGS = #-Wall -Wextra -std=c11
LDFLAGS_SERVER = -lopen62541
LDFLAGS_CLIENT = -lopen62541 -lcurl -lcjson

SRC_SERVER = src/main.c src/env_loader.c src/opc_ua_server.c src/sections.c src/log.c
OBJ_SERVER = $(SRC_SERVER:.c=.o)

SRC_CLIENT = src/main_client.c src/env_loader.c src/weather_thread.c src/opc_ua_client.c src/sections.c src/weather_condition.c src/log.c
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

.PHONY: all build doc test server client clean help

BUILD_DIR = build/

TEST_DIR = tests/run_tests

all: server client

doc: 
	cd $(BUILD_DIR) && make doc

build: 
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)logs
	cp -r resources/ $(BUILD_DIR)
	test -f ".env" && cp .env $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make

test:
	./$(BUILD_DIR)$(TEST_DIR)

server: $(OBJ_SERVER)
	$(CC) $(CFLAGS) $(OBJ_SERVER) -o server_opc_ua $(LDFLAGS_SERVER)

client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) $(OBJ_CLIENT) -o client_opc_ua $(LDFLAGS_CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_SERVER) $(OBJ_CLIENT) server client

help:
	@echo "Makefile Usage:"
	@echo "  make          - Build both server and client"
	@echo "  make build    - Build project by cmake in $(BUILD_DIR)"
	@echo "  make server   - Build only the server"
	@echo "  make client   - Build only the client"
	@echo "  make test     - Run tests all for repository"
	@echo "  make clean    - Remove compiled files"
	@echo "  make doc      - Generate documentation using Doxygen"
	@echo "  make help     - Show this help message"
