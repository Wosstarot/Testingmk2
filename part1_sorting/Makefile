CXX = g++
BUILD ?= release

CXXFLAGS_COMMON = -std=c++17 -Wall -Wextra -I src -MMD -MP

ifeq ($(BUILD),debug)
  CXXFLAGS = $(CXXFLAGS_COMMON) -O0 -g
else
  CXXFLAGS = $(CXXFLAGS_COMMON) -O3 -march=native -DNDEBUG
endif

# Detect OS for cross-platform commands
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    RM = if exist $(subst /,\,$(1)) rmdir /s /q $(subst /,\,$(1))
    DEL = if exist $(1) del /q $(1)
    EXE = .exe
else
    MKDIR = mkdir -p $(1)
    RM = rm -rf $(1)
    DEL = rm -f $(1)
    EXE =
endif

BUILD_DIR = build/$(BUILD)

# Executables
BENCHMARK = $(BUILD_DIR)/benchmark$(EXE)
TESTS = $(BUILD_DIR)/test_sorting$(EXE)

# Source files
SRC = src/bubble.cpp \
      src/counting.cpp \
      src/heap.cpp \
      src/insertion.cpp \
      src/merge.cpp \
      src/quick.cpp \
      src/radix.cpp \
      src/selection.cpp

SRC_OBJS = $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)

# Default target: build benchmark
all: $(BENCHMARK)

# Test target: build and run tests
test: $(TESTS)
	./$(TESTS)

# Build benchmark executable
$(BENCHMARK): $(SRC_OBJS) $(BUILD_DIR)/benchmark.o $(BUILD_DIR)/generator.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build test executable
$(TESTS): $(SRC_OBJS) $(BUILD_DIR)/test_sorting.o $(BUILD_DIR)/generator.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile rules
$(BUILD_DIR)/%.o: src/%.cpp
	@$(call MKDIR,$(BUILD_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: tests/%.cpp
	@$(call MKDIR,$(BUILD_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build configs
debug:
	$(MAKE) BUILD=debug

release:
	$(MAKE) BUILD=release

# Clean
clean:
	@$(call RM,build)
	@$(call DEL,benchmark_results.csv)

clean-debug:
	@$(call RM,build/debug)

clean-release:
	@$(call RM,build/release)
	@$(call DEL,benchmark_results.csv)

# Pull in generated dependency files
-include $(BUILD_DIR)/*.d

.PHONY: all test debug release clean clean-debug clean-release
