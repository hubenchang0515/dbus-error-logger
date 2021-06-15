.PHONY: all clean
TARGET := dbus-error-logger
CFLAGS := -std=c99 -O3 `pkg-config --cflags dbus-1`
LDFLAGS := `pkg-config --libs dbus-1`

OBJS := main.o monitor.o message.o method_call_cache.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: monitor.h method_call_cache.h message.h
monitor.o: monitor.h
message.o: message.h
method_call_cache.o: method_call_cache.h

clean:
	rm -f *.o