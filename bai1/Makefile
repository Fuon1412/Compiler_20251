# ===============================
# Makefile for word index program
# Author: Do Manh Phuong - 20225660
# ===============================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = indexer
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# ---- Run with parameters ----
# Usage:
#   make run input=file.txt stop=stopw.txt output=out.txt
run: $(TARGET)
	./$(TARGET) $(input) $(stop) $(output)

clean:
	rm -f $(TARGET) *.o

zip:
	zip -r DoManhPhuong_20225660.zip . -x "*.zip" "*.o" "$(TARGET)"
