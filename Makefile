OPEN_WBO_PATH = ../newopenwbo/open-wbo
CSV_PARSER_PATH = ../CSVparser

EXEC = TimeTabler

BIN_DIR = bin
INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = tests

CC = g++
CC_FLAGS = --std=c++11 -O2 -Wno-literal-suffix
TEST_FLAGS = -lgtest -pthread
LIB_FLAGS = -lopen-wbo -lyaml-cpp
INCLUDE_PATH = -I $(INCLUDE_DIR) -I $(OPEN_WBO_PATH) -I $(OPEN_WBO_PATH)/solvers/minisat2.2 -I $(CSV_PARSER_PATH)
LIB_PATH = -L $(OPEN_WBO_PATH)

EXEC_FULL_PATH = $(BIN_DIR)/$(EXEC)
TEST_EXEC_FULL_PATH = $(BIN_DIR)/test

OBJ_LIST = classroom.o course.o instructor.o is_minor.o program.o segment.o slot.o cclause.o \
			clauses.o constraint_adder.o constraint_encoder.o parser.o time_tabler.o tsolver.o \
			CSVparser.o utils.o custom_parser.o data.o

TEST_OBJ_LIST = test_clauses.o

MAIN_OBJ = $(BIN_DIR)/main.o
MAIN_TEST_OBJ = $(BIN_DIR)/tests_main.o

OBJS = $(addprefix $(BIN_DIR)/, $(OBJ_LIST))
TEST_OBJS = $(addprefix $(BIN_DIR)/, $(TEST_OBJ_LIST))

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

$(BIN_DIR)/%.o: $(CSV_PARSER_PATH)/%.cpp
	@mkdir -p $(BIN_DIR)
	@echo "Compiling "$<"..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -c $< -o $@ $(LIB_FLAGS)

$(BIN_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BIN_DIR) 
	@echo "Compiling "$<"..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -c $< -o $@ $(LIB_FLAGS) $(TEST_FLAGS)

$(EXEC_FULL_PATH): $(OBJS) $(MAIN_OBJ)
	@echo "Building executable..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -o $@ $^ $(LIB_FLAGS)

$(TEST_EXEC_FULL_PATH): $(OBJS) $(MAIN_TEST_OBJ) $(TEST_OBJS)
	@echo "Building executable..."
	@$(CC) $(CC_FLAGS) $(INCLUDE_PATH) $(LIB_PATH) -o $@ $^ $(LIB_FLAGS) $(TEST_FLAGS)

test: $(TEST_EXEC_FULL_PATH)

clean:
	@echo "Cleaning up..."
	@rm -r $(BIN_DIR)