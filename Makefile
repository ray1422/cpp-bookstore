# self documenting makefile with auto resolve dependencies

SRC_DIR=src
BUILD_DIR=build
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEBUG_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.debug.o, $(SOURCES))
DEPS = $(OBJECTS:.o=.d)
DEBUG_DEPS = $(DEBUG_OBJECTS:.debug.o=.debug.d)
CXXFLAGS_DEBUG = -std=c++17 -Wall -Wextra -Werror -pedantic -g -fsanitize=address -fsanitize=undefined
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -O3
CXXLDFLAGS = -lasan -lubsan
POSTCOMPILE = touch $@ $<
BIN_DIR=bin

debug: $(BIN_DIR)/debug ## Build debug version
main: $(BIN_DIR)/main	## Build release version

# Self Documenting Makefile from https://www.client9.com/self-documenting-makefiles/
help:
	@awk -F ':|##' '/^[^\t].+?:.*?##/ {\
	printf "\033[36m%-30s\033[0m %s\n", $$1, $$NF \
	}' $(MAKEFILE_LIST)

$(BIN_DIR)/debug: $(DEBUG_OBJECTS)  $(DEBUG_DEPS) $(BIN_DIR)
	$(CXX) $(CXXLDFLAGS) $(DEBUG_OBJECTS) -o $@

$(BIN_DIR)/main: $(OBJECTS)  $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CXXLDFLAGS) $^ -o $@

$(BUILD_DIR)/%.debug.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.debug.d
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@  -MMD -MP -MF $(BUILD_DIR)/$*.debug.d
	$(POSTCOMPILE)

%.o: %.cpp %.d
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MT $@ -MMD -MP -MF $*.d
	$(POSTCOMPILE)
	

clean: $(BUILD_DIR) ## Clean build directory and binaries
	rm -f $(OBJECTS) $(DEPS) $(DEBUG_OBJECTS) $(DEBUG_DEPS) $(BIN_DIR)/main $(BIN_DIR)/debug

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.debug.d: $(BUILD_DIR);

$(BUILD_DIR)/%.d: $(BUILD_DIR);

-include $(DEPS)
-include $(DEBUG_DEPS)