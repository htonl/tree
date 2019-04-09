IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=.
LDIR =.

LIBS=-lm

_DEPS = tree.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = tree.o tree_driver.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tree_driver: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
