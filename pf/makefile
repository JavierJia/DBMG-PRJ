
include ../makefile.inc

all: libpf.a pftest my_pftest

# lib file dependencies
libpf.a: libpf.a(pf.o pf_filenode.o pf_filelist.o $(CODEROOT)/util/libutil.a)  # and possibly other .o files

# c file dependencies
pf.o: pf.h pf_filenode.h pf_filelist.h 

pf_filenode.o: pf_filenode.h 

pf_filelist.o: pf_filelist.h 

pftest.o: pf.h

# binary dependencies
pftest: pftest.o libpf.a $(CODEROOT)/util/libutil.a

my_pftest.o: pf.h

my_pftest: my_pftest.o libpf.a $(CODEROOT)/util/libutil.a

# dependencies to compile used libraries
.PHONY: $(CODEROOT)/pf/libpf.a #$(CODEROOT)/util/libutil.a
$(CODEROOT)/pf/libpf.a:
	$(MAKE) -C $(CODEROOT)/pf libpf.a
$(CODEROOT)/util/libutil.a:
	$(MAKE) -C $(CODEROOT)/util libutil.a

.PHONY: clean
clean:
	-rm my_pftest pftest *.a *.o *~ tags .storedfilelist.dat new_file.data
	$(MAKE) -C $(CODEROOT)/util clean
