CC = clang-3.6

CFLAGS = -g -Wall -Wextra -std=c11

LDFLAGS = -lcomedi -lm

ELEVSRC = elev.c io.c main.c hwInterface.c doorTimer.c liftCtrl.c

TARGET = heis

all: $(TARGET)

ELEVOBJ = $(ELEVSRC:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(TARGET): $(ELEVOBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(ELEVOBJ)

rebuild:	clean all

.PHONY: all rebuild clean
