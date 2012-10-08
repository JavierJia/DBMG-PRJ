include ../makefile.inc

all: libutil.a test

libutil.a: libutil.a(logger.o io.o) 

logger.o: logger.h

io.o: io.h

test.o: io.h logger.h

test: test.o libutil.a

.PHONY: $(CODEROOT)/util/libutil.a
$(CODEROOT)/util/libutil.a:
	$(MAKE) -C $(CODEROOT)/util libutil.a

.PHONY: clean
clean:
	-rm test *.a *.o *~
