# Compiler to use
CC = gcc

# Compiler flags
# - Wall      : Enable common compiler warnings
# - Wextra    : Enable additional compiler warnings
# - Wpedantic : Warn about code that doesn't strictly follow the C standard
# - Iinclude  : Tell GCC to look in the "include" folder for header files
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude

# List of all C source files that make up the program
SRC = src\chatbot.c \
      src\token_embeds.c

# Name/location of the final executable
TARGET = build\chatbot.exe

# Tell Make that these are commands/targets, not actual files
.PHONY: all run clean

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Run the program
run: $(TARGET)
	.\$(TARGET)

# Remove the compiled executable
clean:
	del /Q $(TARGET) 2>NUL
