#ifndef GAME_h
#define GAME_h

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72  // Up Arrow
#define DOWN 80 // Down Arrow
#define LEFT 75  // Left Arrow
#define RIGHT 77  // Right Arrow
#define ESC 27  // Escape Key
#define numofrows 25
#define numofcols 75
#define middlecol 40
#define middleline 12


typedef struct Pointstruct {
	int x;
	int y;
} Point;


//Func declerations
void mainMenu(int *choice);
void boardDrawing(void);
void preGame(void);
void playingTime(Point snake[], char* c, int* snakesize, int* numoffruitsleft, Point* fruit);
void fruitInserting(Point snake[], int* numoffruitsleft, int* snakesize, Point* fruit);
Point* reallocation(Point *p, int * size);
bool fruitEating(Point snake[], Point* fruit, int* numoffruitsleft);
bool selfbump(Point snake[], int snakesize);
bool wallbump(Point snake[]);
int getKey(void);
void gotoxy(int y, int x);
void cleanscreen(void);
bool fruittouchedges(int fruitrow, int fruitcol);
bool fruittouchsnake(Point snake[], int fruitrow, int fruitcol, int* snakesize);




#endif
