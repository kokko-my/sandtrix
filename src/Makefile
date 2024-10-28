CC = gcc
CFLAGS = -Wall $(COPTIONS)
LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer

TARGET = sand
OBJS = $(TARGET).o system.o window.o mino.o
HEADS = header.h

debug: CPPFLAGS =
debug: COPTIONS = -g3
debug: $(TARGET)

release: CPPFLAGS = -DNDEBUG
release: COPTIONS = -O3
release: $(TARGET)

$(TARGET): $(OBJS)
$(OBJS): $(HEADS)

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJS) core *~