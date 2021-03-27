CC = gcc

all:
	$(CC) gcc `pkg-config --cflags gtk+-3.0` -o ziguiridum main.c `pkg-config --libs gtk+-3.0`

clean:
	rm -rf *.o vumetro
