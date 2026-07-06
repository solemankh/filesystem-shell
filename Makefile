CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c \
      src/shell.c \
	  src/commands.c \
	  src/fs.c
	  

OBJ = $(SRC:.c=.o)

TARGET = filesystem-shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)	
