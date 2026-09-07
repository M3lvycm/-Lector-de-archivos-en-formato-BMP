WARNINGS = -Wall
INCLUDES = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lSDL2

all: main.o pintor.o bmp.o
	gcc main.o pintor.o bmp.o -o main $(LIBS)

main.o: main.c pintor.h bmp.h
	gcc $(WARNINGS) $(INCLUDES) -c main.c

pintor.o: pintor.c pintor.h
	gcc $(WARNINGS) $(INCLUDES) -c pintor.c

bmp.o: bmp.h bmp.c
	gcc $(WARNINGS) $(INCLUDES) -c bmp.c

clean:
	rm -f *.o main
