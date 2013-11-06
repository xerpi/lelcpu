TARGET = lelcpu
OBJS = main.o lelcpu.o

CFLAGS = -O2 -Wall -Werror

all: $(OBJS)
	gcc -o $(TARGET) $(OBJS)

%.o : %.c
	gcc -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJS) $(TARGET)
