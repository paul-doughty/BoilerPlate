TARGET := build/hello_sdl
BUILD_DIR := build
SRC_DIR := src

CXX ?= g++
CXXFLAGS ?= -std=c++14 -O2
CXXFLAGS += -Isrc -Wall -Wextra -Wpedantic -MMD -MP

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs)

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean run debug release

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) $(SDL2_LIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS := $(filter-out -O2,$(CXXFLAGS)) -g3 -O0

debug: clean all

release: all

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
