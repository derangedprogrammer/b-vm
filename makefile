CC          = gcc
CFLAGS      = -std=c99 -O0 -Wall -Wextra -Wpedantic -g -MMD -MP
LDFLAGS     = -lncurses

# --- Directories ---
SRC_DIR     = src
OUT_DIR     = out

# --- Files ---
TARGET      = bvm
SRCS        = $(wildcard $(SRC_DIR)/*.c)
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
DEPS        = $(OBJS:.o=.d)

# --- Rules ---

all: $(TARGET)

# only runs if objects are newer than target
$(TARGET): $(OBJS)
	@echo "  LD    $@"
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# compiler: only runs if .c or .h files changed
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Create out directory if it doesn't exist
$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

# Clean up
clean:
	@echo "  CLEAN $(OUT_DIR) $(TARGET)"
	@rm -rf $(OUT_DIR) $(TARGET)

# Include dependency files (magic that handles header changes)
-include $(DEPS)

.PHONY: all clean
