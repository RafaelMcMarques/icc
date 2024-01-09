TARGET  = matmult

SRCDIRS = ./src
INCDIRS = ./src

CC		= gcc
CFILES		= $(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
CFLAGS      = -O3 -mavx -march=native -I${LIKWID_INCLUDE} -DLIKWID_PERFMON
LDFLAGS		= -lm $(foreach D, $(INCDIRS), -I$(D)) -L${LIKWID_LIB}/lib -llikwid
OBJFILES	= $(patsubst %.c, %.o, $(CFILES))

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	@echo "Limpando ...."
	-rm -rf $(TARGET) $(OBJFILES)

purge: clean
	@echo "Faxina ...."
	-rm -f *~
	@rm -f *.png marker.out
