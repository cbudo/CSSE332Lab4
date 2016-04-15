CC = gcc
CFLAGS = -Wall -c -g 
LDFLAGS = -Wall -g 
PROG = signals

all: clean $(PROG)

$(PROG): $(PROG).o 
	$(CC) $(LDFLAGS) $(PROG).o -o $(PROG)

$(PROG).o: $(PROG).c $(PROG).h
	$(CC) $(CFLAGS) $(PROG).c -c -o $(PROG).o

clean:
	rm -rf $(PROG) $(PROG).o
