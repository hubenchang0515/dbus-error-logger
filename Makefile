.PHONY: all clean
TARGET := dbus-error-logger
CFLAGS := -O3 `pkg-config --cflags dbus-1`
LDFLAGS := `pkg-config --libs dbus-1`

OBJS := main.o monitor.o message.o message_cache.o proc.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: monitor.h message.h message_cache.h proc.h
monitor.o: monitor.h
message.o: message.h
method_call_cache.o: message_cache.h
proc.o: proc.h

clean:
	rm -f *.o