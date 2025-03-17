# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INC_DIR)
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include
	
# Source files
SRCS_DNS = $(SRC_DIR)/dns_proxy.c $(SRC_DIR)/dns_utils.c
#SRCS_CLIENT = $(SRC_DIR)/client.c $(SRC_DIR)/client_utils.c

# Object files
OBJS_DNS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_DNS))
#OBJS_CLIENT = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_CLIENT))

# Target executables
DNS_PROXY = dns_proxy
#CLIENT = client

# Default target
all: $(DNS_PROXY)
#all: $(DNS_PROXY) $(CLIENT)

# Link object files to create the DNS_PROXY
$(DNS_PROXY): $(OBJS_DNS)
	$(CC) $(LDFLAGS) $^ -o $@
	
# Link object files to create the CLIENT
#$(CLIENT): $(OBJS_CLIENT)
#	$(CC) $(LDFLAGS) $^ -o $@

# Compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(DNS_PROXY)
#clean:
#	rm -rf $(OBJ_DIR) $(DNS_PROXY) $(CLIENT)

# Phony targets
.PHONY: all clean

