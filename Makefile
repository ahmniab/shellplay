CC = g++
CFLAGS = -g -Wall 
FTXUI_CFLAGS  = `pkg-config --cflags ftxui`
FTXUI_LDFLAGS = `pkg-config --libs ftxui`
AUDIO_FLAGS = -lsfml-audio -lsfml-system
 
all: run

run: shellplay
	out/shellplay
 
shellplay: main.o
	$(CC) main.o -o out/shellplay 
 
main.o: main.cpp
	$(CC) -c main.cpp -o main.o 
 
clean:
	rm *.o
	rm out/shellplay
