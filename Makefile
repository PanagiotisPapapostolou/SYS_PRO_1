CC = gcc 
CFLAGS = -Wall -I./include
#LDFLAGS =

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
INC_DIR=./include

SRCS_C = $(SRC_DIR)/main.c $(SRC_DIR)/server.c $(SRC_DIR)/string_array.c
OBJS = $(OBJ_DIR)/main.o 
OBJS_1=$(OBJ_DIR)/server.o $(OBJ_DIR)/string_array.o
EXEC = $(BIN_DIR)/jobCommander
EXEC_1=$(BIN_DIR)/jobExecutorServer

.PHONY: all clean

all: $(EXEC) $(EXEC_1)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@	

$(EXEC_1): $(OBJS_1)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/string_array.o: $(SRC_DIR)/string_array.c $(INC_DIR)/string_array.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC) $(EXEC_1)