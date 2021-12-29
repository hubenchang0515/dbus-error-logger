.PHONY: all install clean

all: dbus-error-logger

install: all
	install -m0755 dbus-error-logger /usr/bin


clean: 
	rm -f message.o monitor.o proc.o message_cache.o main.o


dbus-error-logger : message.o monitor.o proc.o message_cache.o main.o  
	gcc -o $@ $^ `pkg-config --libs dbus-1` 

message.o: message.c message.h /usr/include/dbus-1.0/dbus/dbus.h \
 /usr/lib/x86_64-linux-gnu/dbus-1.0/include/dbus/dbus-arch-deps.h \
 /usr/include/dbus-1.0/dbus/dbus-macros.h \
 /usr/include/dbus-1.0/dbus/dbus-address.h \
 /usr/include/dbus-1.0/dbus/dbus-types.h \
 /usr/include/dbus-1.0/dbus/dbus-errors.h \
 /usr/include/dbus-1.0/dbus/dbus-protocol.h \
 /usr/include/dbus-1.0/dbus/dbus-bus.h \
 /usr/include/dbus-1.0/dbus/dbus-connection.h \
 /usr/include/dbus-1.0/dbus/dbus-memory.h \
 /usr/include/dbus-1.0/dbus/dbus-message.h \
 /usr/include/dbus-1.0/dbus/dbus-shared.h \
 /usr/include/dbus-1.0/dbus/dbus-misc.h \
 /usr/include/dbus-1.0/dbus/dbus-pending-call.h \
 /usr/include/dbus-1.0/dbus/dbus-server.h \
 /usr/include/dbus-1.0/dbus/dbus-signature.h \
 /usr/include/dbus-1.0/dbus/dbus-syntax.h \
 /usr/include/dbus-1.0/dbus/dbus-threads.h
	gcc -c message.c -O2 -W -Wall `pkg-config --cflags dbus-1` 

monitor.o: monitor.c /usr/include/dbus-1.0/dbus/dbus.h \
 /usr/lib/x86_64-linux-gnu/dbus-1.0/include/dbus/dbus-arch-deps.h \
 /usr/include/dbus-1.0/dbus/dbus-macros.h \
 /usr/include/dbus-1.0/dbus/dbus-address.h \
 /usr/include/dbus-1.0/dbus/dbus-types.h \
 /usr/include/dbus-1.0/dbus/dbus-errors.h \
 /usr/include/dbus-1.0/dbus/dbus-protocol.h \
 /usr/include/dbus-1.0/dbus/dbus-bus.h \
 /usr/include/dbus-1.0/dbus/dbus-connection.h \
 /usr/include/dbus-1.0/dbus/dbus-memory.h \
 /usr/include/dbus-1.0/dbus/dbus-message.h \
 /usr/include/dbus-1.0/dbus/dbus-shared.h \
 /usr/include/dbus-1.0/dbus/dbus-misc.h \
 /usr/include/dbus-1.0/dbus/dbus-pending-call.h \
 /usr/include/dbus-1.0/dbus/dbus-server.h \
 /usr/include/dbus-1.0/dbus/dbus-signature.h \
 /usr/include/dbus-1.0/dbus/dbus-syntax.h \
 /usr/include/dbus-1.0/dbus/dbus-threads.h monitor.h
	gcc -c monitor.c -O2 -W -Wall `pkg-config --cflags dbus-1` 

proc.o: proc.c /usr/include/dbus-1.0/dbus/dbus.h \
 /usr/lib/x86_64-linux-gnu/dbus-1.0/include/dbus/dbus-arch-deps.h \
 /usr/include/dbus-1.0/dbus/dbus-macros.h \
 /usr/include/dbus-1.0/dbus/dbus-address.h \
 /usr/include/dbus-1.0/dbus/dbus-types.h \
 /usr/include/dbus-1.0/dbus/dbus-errors.h \
 /usr/include/dbus-1.0/dbus/dbus-protocol.h \
 /usr/include/dbus-1.0/dbus/dbus-bus.h \
 /usr/include/dbus-1.0/dbus/dbus-connection.h \
 /usr/include/dbus-1.0/dbus/dbus-memory.h \
 /usr/include/dbus-1.0/dbus/dbus-message.h \
 /usr/include/dbus-1.0/dbus/dbus-shared.h \
 /usr/include/dbus-1.0/dbus/dbus-misc.h \
 /usr/include/dbus-1.0/dbus/dbus-pending-call.h \
 /usr/include/dbus-1.0/dbus/dbus-server.h \
 /usr/include/dbus-1.0/dbus/dbus-signature.h \
 /usr/include/dbus-1.0/dbus/dbus-syntax.h \
 /usr/include/dbus-1.0/dbus/dbus-threads.h proc.h
	gcc -c proc.c -O2 -W -Wall `pkg-config --cflags dbus-1` 

message_cache.o: message_cache.c message_cache.h \
 /usr/include/dbus-1.0/dbus/dbus.h \
 /usr/lib/x86_64-linux-gnu/dbus-1.0/include/dbus/dbus-arch-deps.h \
 /usr/include/dbus-1.0/dbus/dbus-macros.h \
 /usr/include/dbus-1.0/dbus/dbus-address.h \
 /usr/include/dbus-1.0/dbus/dbus-types.h \
 /usr/include/dbus-1.0/dbus/dbus-errors.h \
 /usr/include/dbus-1.0/dbus/dbus-protocol.h \
 /usr/include/dbus-1.0/dbus/dbus-bus.h \
 /usr/include/dbus-1.0/dbus/dbus-connection.h \
 /usr/include/dbus-1.0/dbus/dbus-memory.h \
 /usr/include/dbus-1.0/dbus/dbus-message.h \
 /usr/include/dbus-1.0/dbus/dbus-shared.h \
 /usr/include/dbus-1.0/dbus/dbus-misc.h \
 /usr/include/dbus-1.0/dbus/dbus-pending-call.h \
 /usr/include/dbus-1.0/dbus/dbus-server.h \
 /usr/include/dbus-1.0/dbus/dbus-signature.h \
 /usr/include/dbus-1.0/dbus/dbus-syntax.h \
 /usr/include/dbus-1.0/dbus/dbus-threads.h
	gcc -c message_cache.c -O2 -W -Wall `pkg-config --cflags dbus-1` 

main.o: main.c /usr/include/dbus-1.0/dbus/dbus.h \
 /usr/lib/x86_64-linux-gnu/dbus-1.0/include/dbus/dbus-arch-deps.h \
 /usr/include/dbus-1.0/dbus/dbus-macros.h \
 /usr/include/dbus-1.0/dbus/dbus-address.h \
 /usr/include/dbus-1.0/dbus/dbus-types.h \
 /usr/include/dbus-1.0/dbus/dbus-errors.h \
 /usr/include/dbus-1.0/dbus/dbus-protocol.h \
 /usr/include/dbus-1.0/dbus/dbus-bus.h \
 /usr/include/dbus-1.0/dbus/dbus-connection.h \
 /usr/include/dbus-1.0/dbus/dbus-memory.h \
 /usr/include/dbus-1.0/dbus/dbus-message.h \
 /usr/include/dbus-1.0/dbus/dbus-shared.h \
 /usr/include/dbus-1.0/dbus/dbus-misc.h \
 /usr/include/dbus-1.0/dbus/dbus-pending-call.h \
 /usr/include/dbus-1.0/dbus/dbus-server.h \
 /usr/include/dbus-1.0/dbus/dbus-signature.h \
 /usr/include/dbus-1.0/dbus/dbus-syntax.h \
 /usr/include/dbus-1.0/dbus/dbus-threads.h monitor.h message.h \
 message_cache.h proc.h
	gcc -c main.c -O2 -W -Wall `pkg-config --cflags dbus-1` 

