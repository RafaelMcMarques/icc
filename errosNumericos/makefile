TARGET  = ep01

SRCDIRS = ./src
INCDIRS = ./src

CC		= gcc
CFILES		= $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
CFLAGS		= -Wall -Wextra -pedantic
LDFLAGS		= -lm $(foreach D, $(INCDIRS), -I$(D))
OBJFILES	= $(patsubst %.c, %.o, $(CFILES))

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	-rm -rf $(TARGET) $(OBJFILES)
