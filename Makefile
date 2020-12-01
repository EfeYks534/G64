CC = gcc
CFLAGS = -Wall -Wpedantic -fPIC -g
OBJECTS = File.o Hash.o Lex.o Random.o \
		StringBuilder.o TimeUtil.o Vector.o

all: $(OBJECTS) lib

lib: $(OBJECTS)
	mkdir -p build
	ar rcs build/libG64.a $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o build/libG64.so
	chmod -X build/libG64.so
	sudo cp build/libG64.a /usr/lib/libG64.a
	sudo cp build/libG64.so /usr/lib/libG64.so
	sudo cp *.h /usr/include/G64/

clean:
	rm -r build
	rm $(OBJECTS)

File.o: File.c FileLib.h
		$(CC) -c File.c $(CFLAGS)

Hash.o: Hash.c HashLib.h
		$(CC) -c Hash.c $(CFLAGS)

Lex.o: Lex.c LexLib.h
		$(CC) -c Lex.c $(CFLAGS)

Random.o: Random.c RndLib.h
		$(CC) -c Random.c $(CFLAGS)

StringBuilder.o: StringBuilder.c StringLib.h
		$(CC) -c StringBuilder.c $(CFLAGS)

TimeUtil.o: TimeUtil.c TimeLib.h
		$(CC) -c TimeUtil.c $(CFLAGS)

Vector.o: Vector.c VectorLib.h
		$(CC) -c Vector.c $(CFLAGS)
