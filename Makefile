CC      = gcc
CFLAGS  = -std=gnu11 -Wall -Wextra -Werror -Wno-unused-parameter -O2 -g -pthread
LDFLAGS = -pthread

TARGET = sync_car
OBJS   = sync_car.o sync_car-runner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

sync_car.o: sync_car.c sync_car.h
	$(CC) $(CFLAGS) -c sync_car.c -o $@

sync_car-runner.o: sync_car-runner.c sync_car.h
	$(CC) $(CFLAGS) -c sync_car-runner.c -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
