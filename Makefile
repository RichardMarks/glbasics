CC ?= gcc
CFLAGS ?= $(shell pkg-config glew glfw3 --cflags) -O0
LDFLAGS ?= $(shell pkg-config glew glfw3 --libs) -Wl,-headerpad_max_install_names -framework OpenGL

TARGET_EXEC ?= game
BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -g

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
MKDIR_P ?= mkdir -p
