CC=g++
CFLAGS=-std=c++14 -O3 -w

compresscdi:
	$(CC) $(CFLAGS) -o compresscdi compresscdi.cpp

uncompresscdi:
	$(CC) $(CFLAGS) -o uncompresscdi uncompresscdi.cpp


all: compresscdi uncompresscdi

clean:
	rm -f compresscdi uncompresscdi