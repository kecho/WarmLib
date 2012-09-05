## Kleber Garcia (c) 2012
## this file requires a predefinition of TARGET and SRC_FILES
## SRC_FILES assumes the root is the source directory of this project

CC = g++
AR = ar

BIN_PATH = $(LEVEL)/build
SRC_PATH = $(LEVEL)/../source/$(TARGET)
INCLUDE_PATHS = $(LEVEL)/../source/ $(SRC_PATH)
ABS_SRC_FILES = $(addprefix $(SRC_PATH)/, $(SRC_FILES)) 
NO_DIR_SRC_FILES = $(notdir $(ABS_SRC_FILES))
OBJ_FILES = $(NO_DIR_SRC_FILES:.cpp=.o)
LIB_NAME = $(BIN_PATH)/lib$(TARGET).a

$(BIN_PATH) :
	mkdir $(BIN_PATH)

$(OBJ_FILES) : $(ABS_SRC_FILES)
	$(CC) -c $(addprefix -I, $(INCLUDE_PATHS)) $(ABS_SRC_FILES)

$(LIB_NAME) : $(BIN_PATH) $(OBJ_FILES)
	$(AR) cvr $(LIB_NAME) $(OBJ_FILES) 

clean :
	rm *.o
