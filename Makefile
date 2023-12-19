SOBJ=   $(PACKSODIR)/plblas.$(SOEXT)

all:    $(SOBJ)

OBJ=	cpp/plblas.o

%.o:	%.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $@

$(SOBJ): $(OBJ)
	mkdir -p $(PACKSODIR)
	$(LD) $(LDSOFLAGS) -o $@ $(OBJ) $(SWISOLIB) -larmadillo

check::

install::

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(SOBJ)
