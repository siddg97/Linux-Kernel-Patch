CC = gcc
CFLAGS = -std=c99 -static

TARGET = process_ancestors_test

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(TARGET).c $(CFLAGS) -o $(TARGET)

transfer:
	scp -P 2222 $(TARGET) root@localhost:~

clean:
	$(RM) $(TARGET)