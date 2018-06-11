SUFFIXES += .d
NODEPS := clean distclean

TARGET := rbgParser
SRC_DIR := src
INC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
DEP_DIR := dep
EXAMPLES_DIR := examples

C := g++
INCLUDE := -I$(INC_DIR)
CFLAGS := -Wall -Wextra -Wpedantic -O3 -flto -std=c++11 -s $(INCLUDE)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
DEPFILES := $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d, $(wildcard $(SRC_DIR)/*.cpp))
EXAMPLES := $(wildcard $(EXAMPLES_DIR)/*.rbg)

all: $(TARGET)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEPFILES)
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp | $(DEP_DIR)
	$(C) $(CFLAGS) -MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<) $@' $< -MF $@

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(C) $(CFLAGS) $(OBJECTS) -o $(BIN_DIR)/$@

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

verify: $(EXAMPLES)
	@$(foreach game,$(EXAMPLES),                     \
		echo 'Verifying idempotency on $(game)...';  \
		$(BIN_DIR)/$(TARGET) -o test1.rbg $(game);   \
		$(BIN_DIR)/$(TARGET) -o test2.rbg test1.rbg; \
		diff test1.rbg test2.rbg;                    \
		rm test*.rbg;                                \
		echo "";)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

distclean: clean
	rm -rf $(BIN_DIR)
