TARGET = simp_network
CFLAGS = -g -std=c99 $(shell pkg-config --cflags gtk+-3.0)
LIBS   = $(shell pkg-config --libs gtk+-3.0)
LIBS  += -lm

OBJS   = $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) $(TARGET) $(OBJS)

