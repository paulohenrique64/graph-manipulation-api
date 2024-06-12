SRC_DIR := ./src ./include
BUILD_DIR := ./build
FINAL_BIN := project_run

# find all source codes
SRCS := $(shell find $(SRC_DIR) -name *.cpp)

# objects final name
OBJS := $(addsuffix .o, $(SRCS))
OBJS := $(addprefix $(BUILD_DIR)/, $(OBJS))

# find all directories do pass to compiler with -I flag
DEP_DIRECTORIES := $(shell find $(SRC_DIR) -type d)
DEP_FLAG := $(addprefix -I, $(DEP_DIRECTORIES))

# final build step
$(BUILD_DIR)/$(FINAL_BIN): $(OBJS)
	$(CXX) $^ -o $@
	@echo 'BUILD SUCESS'

# middle build step
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $(DEP_FLAG) $? -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)