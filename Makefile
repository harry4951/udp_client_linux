CC = gcc
LIBS = -lm -ljack
OBJ = inoutput2.o

inoutput: $(OBJ)
	$(CC) -o inoutput2 inoutput2.c $(LIBS)


clean:
	rm -f *.o

 
run:
	./inoutput2
