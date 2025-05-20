# Target  name project executable file
TARGET = project1

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
LIB_SHARED_DIR := $(LIB_DIR)/shared



# Source static file
SRC_STATIC_FILES := math.c hello.c
SRC_STATIC_PATH := $(addprefix $(SRC_DIR)/, $(SRC_STATIC_FILES))


# Source shared file
SRC_SHARED_FILES := d1.c d2.c
SRC_SHARED_PATH := $(addprefix $(SRC_DIR)/, $(SRC_SHARED_FILES))

LIB := $(basename $(SRC_STATIC_FILES)) $(basename $(SRC_SHARED_FILES))
# Object file
OBJ_STATIC_FILES := $(SRC_STATIC_FILES:.c=.o)
OBJ_STATIC_PATH := $(addprefix $(OBJ_DIR)/, $(OBJ_STATIC_FILES))

OBJ_SHARED_FILES := $(SRC_SHARED_FILES:.c=.o)
OBJ_SHARED_PATH := $(addprefix $(OBJ_DIR)/, $(OBJ_SHARED_FILES))

OBJ_PATH := $(OBJ_SHARED_PATH) $(OBJ_STATIC_FILES)






# console log path
log: 
	@echo $(OBJ_SHARED_FILES)

# create shared library config path using LD_LIBRARY_PATH
$(OBJ_SHARED_PATH):
	gcc -c -fPIC $(SRC_DIR)/$(basename $(notdir $@)).c -o $@ -I$(INC_DIR)

$(OBJ_SHARED_FILES): $(OBJ_SHARED_PATH)
	gcc -shared $(OBJ_DIR)/$(notdir $@) -o $(LIB_SHARED_DIR)/lib$(basename $(notdir $@)).so

# create static library
$(OBJ_STATIC_PATH):
	gcc -c $(SRC_DIR)/$(basename $(notdir $@)).c -o $@ -I$(INC_DIR)

$(OBJ_STATIC_FILES): $(OBJ_STATIC_PATH)
	ar -cr $(LIB_STATIC_DIR)/lib$(basename $(notdir $@)).a $(OBJ_DIR)/$@

# main object
main.o:
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I$(INC_DIR)

# linking
all: main.o $(OBJ_STATIC_FILES) $(OBJ_SHARED_FILES)
	gcc $(OBJ_DIR)/$< -L$(LIB_SHARED_DIR) -L$(LIB_STATIC_DIR) $(addprefix -l, $(LIB)) -o $(BIN_DIR)/$(TARGET)

clean:
	rm -rf $(BIN_DIR)/main $(OBJ_DIR)/*.o $(LIB_SHARED_DIR)/*.so $(LIB_STATIC_DIR)/*.a


# bai mau

# create_obj:
# 	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I$(INC_DIR) 
# 	gcc -c -fPIC $(SRC_DIR)/math.c -o $(OBJ_DIR)/math.o -I$(INC_DIR)
# 	gcc -c -fPIC $(SRC_DIR)/hello.c -o $(OBJ_DIR)/hello.o -I$(INC_DIR)

# create_shared:
# 	gcc -shared $(OBJ_DIR)/math.o -o $(LIB_SHARED_DIR)/libmath.so

# linking:
# 	gcc $(OBJ_DIR)/hello.o  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lmath -o $(BIN_DIR)/main

# all: create_obj create_shared linking