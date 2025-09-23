CXX = g++
CXXFLAGS = -std=c++14 -pthread -Werror -Wpedantic -Wall
TARGET = Labwork1
EXPERIMENT_TARGET = experiment

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

TARGET_PATH = $(BIN_DIR)/$(TARGET)
EXPERIMENT_PATH = $(BIN_DIR)/$(EXPERIMENT_TARGET)

all: $(TARGET_PATH) $(EXPERIMENT_PATH)

$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

$(TARGET_PATH): $(OBJ_DIR)/main.o $(OBJ_DIR)/imageFunction.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)/main.o $(OBJ_DIR)/imageFunction.o -o $@

$(EXPERIMENT_PATH): $(OBJ_DIR)/experiment.o $(OBJ_DIR)/imageFunction.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_DIR)/experiment.o $(OBJ_DIR)/imageFunction.o -o $@

$(OBJ_DIR)/main.o: main.cpp imageFunction.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

$(OBJ_DIR)/experiment.o: experiment.cpp imageFunction.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c experiment.cpp -o $@

$(OBJ_DIR)/imageFunction.o: imageFunction.cpp imageFunction.hpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c imageFunction.cpp -o $@

run-experiment: $(EXPERIMENT_PATH)
	./$(EXPERIMENT_PATH)

run: $(TARGET_PATH)
	./$(TARGET_PATH)

tree:
	@echo "Directory structure:"
	@tree $(BUILD_DIR) || find $(BUILD_DIR) | sed 's|[^/]*/|- |g'

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -rf Images

images-dir:
	@mkdir -p Images

all-with-images: images-dir all

.PHONY: all clean distclean run run-experiment tree images-dir all-with-images