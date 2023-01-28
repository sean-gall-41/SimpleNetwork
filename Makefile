TARGET = simp_network
CPP=g++
CPPFLAGS = -g -std=c++11 $(shell pkg-config --cflags gtk+-3.0)
LIBS   = $(shell pkg-config --libs gtk+-3.0)
LIBS  += -lm

OBJS   = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: clean

$(TARGET): $(OBJS)
	$(CPP) -g $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $^

clean:
	$(RM) $(TARGET) $(OBJS)

