uicp: main.c
	gcc  -Wall -pedantic -pthread ./main.c  `pkg-config --libs --cflags raylib` -o uicp

