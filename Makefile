PROGRAM_SENDER := sender
PROGRAM_LISTENER := listener

SENDER_SOURCES := src/sender.c
LISTENER_SOURCES := src/listener.c

CFLAGS := -g -O0

all: $(PROGRAM_SENDER) $(PROGRAM_LISTENER)

$(PROGRAM_SENDER): $(SENDER_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

$(PROGRAM_LISTENER): $(LISTENER_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $+

clean:
	rm -rf $(PROGRAM_SENDER) $(PROGRAM_LISTENER) *.dSYM *.o

.PHONY: all clean

