SOBJ=   $(PACKSODIR)/plblas.$(SOEXT)

all:    $(SOBJ)

OBJ=	cpp/plblas.o

$(SOBJ): $(OBJ)
	mkdir -p $(PACKSODIR)
	$(LD) $(LDSOFLAGS) -o $@ $(OBJ) $(SWISOLIB)

check::

install::

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(SOBJ)
