# Target  name project executable file
TARGET = chat

# Compiler and flags
CXX = gcc
CXXFLAGS = -pthread -g -std=c99 -lm

# Directory file
CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

# Source static library
SRC_STATIC_FILES := queue.c control.c tcpsocket.c gui.c
SRC_STATIC_PATH := $(addprefix $(SRC_DIR)/, $(SRC_STATIC_FILES))

# Object file
OBJ_STATIC_FILES := $(SRC_STATIC_FILES:.c=.o)
OBJ_STATIC_PATH := $(addprefix $(OBJ_DIR)/, $(OBJ_STATIC_FILES))
OBJ_PATH := $(OBJ_STATIC_FILES)

# create object file
$(OBJ_STATIC_PATH):
	gcc -c $(SRC_DIR)/$(basename $(notdir $@)).c -o $@ -I$(INC_DIR) $(CXXFLAGS)
clean:

# main object
main.o:
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I$(INC_DIR) $(CXXFLAGS)
clean:

# linking
all: main.o $(OBJ_STATIC_PATH) 
	gcc -o $(BIN_DIR)/$(TARGET)  $(OBJ_DIR)/$< $(OBJ_STATIC_PATH) $(CXXFLAGS)
clean:
	rm -rf $(BIN_DIR)/main $(OBJ_DIR)/*.o
