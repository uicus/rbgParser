SUFFIXES += .d
NODEPS := clean distclean

TARGET := rbgParser
SRC_DIR := src
INC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
DEP_DIR := dep
MAIN_FILE := $(SRC_DIR)/main.cpp

C := g++
AR = gcc-ar
INCLUDE := -I$(INC_DIR)
CFLAGS := -Wall -Wextra -Wpedantic -O3 -flto -std=c++17 -s $(INCLUDE)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS_FOR_LIB := $(filter-out $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAIN_FILE)), $(OBJECTS))
DEPFILES := $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d, $(wildcard $(SRC_DIR)/*.cpp))

all: $(TARGET) lib$(TARGET).a

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEPFILES)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp | $(DEP_DIR)
	$(C) $(CFLAGS) -MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<) $@' $< -MF $@

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(C) $(CFLAGS) $(OBJECTS) -o $(BIN_DIR)/$@

lib$(TARGET).a: $(OBJECTS_FOR_LIB) | $(BIN_DIR)
	$(AR) rs $(BIN_DIR)/$@ $(OBJECTS_FOR_LIB)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

distclean: clean
	rm -rf $(BIN_DIR)
