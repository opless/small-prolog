HEADERS = prbltin.h prlex.h prlush.h prolog.h prtypes.h
OBJECTS = pralloc.o prassert.o prbltin.o prcnsult.o prdebug.o prerror.o prhash.o pribmpc.o prlush.o prmain.o prparse.o prprint.o prscan.o prunify.o

default: sprolog

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

sprolog: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f sprolog
