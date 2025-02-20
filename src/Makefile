# Define the compiler
CC = gcc

# WARNINGS
CFLAGS = -Wall

# Define the output directory
OUT_DIR = ../bin

# Define the source files
SRCS = HelloWorld.c

# Define the object files (replace .c with .o)
OBJS = $(SRCS:%.c=$(OUT_DIR)/%.o)

# Define the final executable
TARGET = $(OUT_DIR)/HelloWorld

# Rule to compile the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
# Rule to compile each source file into an object file
$(OUT_DIR)/%.o: %.c
	$(CC) -o $@ -c $<

# Rule to create the output directory
$(OBJS): | $(OUT_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean

# Clean up the build
clean:
	rm -rf $(OUT_DIR)

