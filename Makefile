CC = gcc
CFLAGS = -Wall -Wpedantic -fPIC -g
OBJECTS = File.o Hash.o Lex.o Random.o \
		StringBuilder.o TimeUtil.o Vector.o

all: compile ar_a ar_so
	rm $(OBJECTS)

compile: $(OBJECTS)
	mkdir -p build

ar_a: $(OBJECTS)
	ar rcs build/libG64.a $(OBJECTS)

ar_so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o build/libG64.so
	chmod -X build/libG64.so

copy_lib:
	sudo cp build/libG64.a /usr/lib/libG64.a
	sudo cp build/libG64.so /usr/lib/libG64.so
	sudo cp *.h /usr/include/G64/

clean:
	touch $(OBJECTS)
	rm $(OBJECTS)
	mkdir -p build
	rm -r build

File.o: File.c G64.h
		$(CC) -c File.c $(CFLAGS)

Hash.o: Hash.c G64.h
		$(CC) -c Hash.c $(CFLAGS)

Lex.o: Lex.c G64.h
		$(CC) -c Lex.c $(CFLAGS)

Random.o: Random.c G64.h
		$(CC) -c Random.c $(CFLAGS)

StringBuilder.o: StringBuilder.c G64.h
		$(CC) -c StringBuilder.c $(CFLAGS)

TimeUtil.o: TimeUtil.c G64.h
		$(CC) -c TimeUtil.c $(CFLAGS)

Vector.o: Vector.c G64.h
		$(CC) -c Vector.c $(CFLAGS)
