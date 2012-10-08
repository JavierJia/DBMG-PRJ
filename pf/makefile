
include ../makefile.inc

all: libpf.a pftest

# lib file dependencies
libpf.a: libpf.a(pf.o)  # and possibly other .o files

# c file dependencies
pf.o: pf.h 

pftest.o: pf.h

# binary dependencies
pftest: pftest.o libpf.a $(CODEROOT)/pf/libpf.a $(CODEROOT)/util/libutil.a

# dependencies to compile used libraries
.PHONY: $(CODEROOT)/pf/libpf.a $(CODEROOT)/util/libutil.a
$(CODEROOT)/pf/libpf.a:
	$(MAKE) -C $(CODEROOT)/pf libpf.a
$(CODEROOT)/util/libutil.a:
	$(MAKE) -C $(CODEROOT)/util libutil.a

.PHONY: clean
clean:
	-rm pftest *.a *.o *~
