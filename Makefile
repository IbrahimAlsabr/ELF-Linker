CC=gcc  #compiler
TARGET=main #target file name

all:
	$(CC) main.c readfile.c printinfo.c  -o $(TARGET)

clean:
	rm $(TARGET)

