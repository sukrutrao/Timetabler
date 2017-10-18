OPEN_WBO_PATH = ../newopenwbo/open-wbo
CSV_PARSER_PATH = ../CSVparser
YAML_CPP_PATH = 2
EXEC = TimeTabler

BIN_DIR = bin
INCLUDE_DIR = include
SRC_DIR = src

CC = g++
CC_FLAGS = --std=c++11
LIB_FLAGS = -lopen-wbo
INCLUDE_PATH = -I $(INCLUDE_DIR) -I $(OPEN_WBO_PATH) -I $(OPEN_WBO_PATH)/solvers/minisat2.2 -I $(CSV_PARSER_PATH)
LIB_PATH = -L $(OPEN_WBO_PATH)

EXEC_FULL_PATH = $(BIN_DIR)/$(EXEC)

OBJ_LIST = classroom.o course.o instructor.o is_minor.o program.o segment.o slot.o cclause.o \
			clauses.o constraint_adder.o encoder.o parser.o time_tabler.o tsolver.o utils.o main.o

OBJS = $(addprefix $(BIN_DIR)/, $(OBJ_LIST))

.PHONY: all clean

all: $(EXEC_FULL_PATH)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR) 
	@echo "Compiling "$<"..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -c $< -o $@ $(LIB_FLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/fields/%.cpp
	@mkdir -p $(BIN_DIR)
	@echo "Compiling "$<"..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -c $< -o $@ $(LIB_FLAGS)

$(EXEC_FULL_PATH): $(OBJS)
	@echo "Building executable..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -o $@ $^ $(LIB_FLAGS)

clean:
	@echo "Cleaning up..."
	@rm -r $(BIN_DIR)