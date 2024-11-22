# Compiler and flags
CC = gcc
CFLAGS = -Wall -I. -I./cJSON # Add cJSON header path
LDFLAGS = -L./cJSON/build -lcjson # Link the cJSON library

# Targets
TARGET = weather_program
SRC = main.c weather.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Compiling source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)
