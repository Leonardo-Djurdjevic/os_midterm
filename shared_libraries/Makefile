CFLAGS = -Wall -fPIC

TARGET = libshared.so

.PHONY: all
all: $(TARGET)

$(TARGET): add mult shared_libraries
	$(CC) -shared -o $(TARGET) add.o mult.o shared_libraries.o

add: add.c
	$(CC) $(CFLAGS) -c add.c -o add.o

mult: mult.c
	$(CC) $(CFLAGS) -c mult.c -o mult.o

shared_libraries: shared_libraries.c
	$(CC) $(CFLAGS) -c shared_libraries.c -o shared_libraries.o

.PHONY: clean
clean:
	rm -f add mult shared_libraries $(TARGET)
