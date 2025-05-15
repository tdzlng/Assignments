# Target  name project executable file
TARGET = chat

# Compiler and flags
CXX = gcc
CXXFLAGS = -std=c++11

# Directory file
CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static

# Source static library
SRC_STATIC_FILES := log.c queue.c control.c tcpsocket.c gui.c
SRC_STATIC_PATH := $(addprefix $(SRC_DIR)/, $(SRC_STATIC_FILES))

#Linking library
LIB := $(basename $(SRC_STATIC_FILES))

# Object file
OBJ_STATIC_FILES := $(SRC_STATIC_FILES:.c=.o)
OBJ_STATIC_PATH := $(addprefix $(OBJ_DIR)/, $(OBJ_STATIC_FILES))
OBJ_PATH := $(OBJ_STATIC_FILES)

# create static library
$(OBJ_STATIC_PATH):
	gcc -c $(SRC_DIR)/$(basename $(notdir $@)).c -o $@ -I$(INC_DIR)

$(OBJ_STATIC_FILES): $(OBJ_STATIC_PATH)
	ar -cr $(LIB_STATIC_DIR)/lib$(basename $(notdir $@)).a $(OBJ_DIR)/$@

# main object
main.o:
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I$(INC_DIR)

# linking
all: main.o $(OBJ_STATIC_FILES) 
	gcc $(OBJ_DIR)/$< -L$(LIB_STATIC_DIR) $(addprefix -l, $(LIB)) -o $(BIN_DIR)/$(TARGET)

clean:
	rm -rf $(BIN_DIR)/main $(OBJ_DIR)/*.o $(LIB_STATIC_DIR)/*.a
