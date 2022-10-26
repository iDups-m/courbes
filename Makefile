CC=g++

CFLAGS= -Wall -I/urs/local/include -lGL -lGLEW -lGLU -lglut  -lm 
LDFLAGS= -larmadillo

EXEC=generateCurves

all: $(EXEC)

generateCurves: generateCurves.o
	$(CC) -o $(EXEC) generateCurves.o $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
