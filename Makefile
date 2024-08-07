CC = g++
CFLAGS = -g -Wall 
FTXUI_CFLAGS  = `pkg-config --cflags ftxui`
FTXUI_LDFLAGS = `pkg-config --libs ftxui`
AUDIO_FLAGS = -lsfml-audio -lsfml-system
 
all: run

run: main
	./main
 
main: main.o
	$(CC) main.o -o main 
 
main.o: main.cpp
	$(CC) -c main.cpp -o main.o 
 
clean:
	rm main.o
	rm main
