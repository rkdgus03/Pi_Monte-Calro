CC = mpicc
CFLAGS = -I/home/sprng5/include -Wall -O2
LDFLAGS = -L/home/sprng5/lib -lsprng -lm

TARGET = monte_carlo_sprng
SRC = 241201.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)