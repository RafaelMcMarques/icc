TARGET  = ajustePol

SRCDIRS = ./src
INCDIRS = ./src

LIKWID_HOME = /home/soft/likwid
CC		= gcc
CFILES		= $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
CFLAGS      = -O3 -mavx -march=native -I${LIKWID_INCLUDE} -DLIKWID_PERFMON
LDFLAGS		= -lm $(foreach D, $(INCDIRS), -I$(D)) -L${LIKWID_LIB} -llikwid
OBJFILES	= $(patsubst %.c, %.o, $(CFILES))

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	-rm -rf $(TARGET) $(OBJFILES)

purge: clean
	-rm -f *~
