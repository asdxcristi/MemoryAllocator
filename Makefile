CC=gcc
TARGET = allocator
build: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o allocator $(TARGET).c -lm

clean:
	$(RM) $(TARGET)
