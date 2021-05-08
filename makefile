CC = gcc

all:
	$(CC) -o ziguiridum main.c `pkg-config --libs gtk+-3.0 --cflags --libs gstreamer-1.0`

clean:
	rm -rf *.o vumetro
