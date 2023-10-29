#include "game.h"


void mainMenu(int* choice){
	int mychoice;
	//Getting player's choice regarding game level
	do {
		printf("Welcome to Snake!\nChoose level:\n0. Exit\n1. Basic\n2. Medium\n3. Advanced\n");
		scanf_s("%d", &mychoice);
		(*choice) = mychoice;
		if ((*choice) < 0 || (*choice) > 3)
			printf("Illegal number, try again.\n");
	}
	while ((*choice) < 0 || (*choice) > 3);//won't stop until a valid choice is inserted
}


void boardDrawing() {
	int i;
	cleanscreen();
	for (i = 0; i < numofcols; i++) {
		//inserting upper line
		gotoxy(i, 0);
		printf("#");
		//inserting lower line
		gotoxy(i, 24);
		printf("#");
	}
	for (i = 0; i < numofrows; i++) {
		//inserting left column
		gotoxy(0, i);
		printf("#");
		//inserting right column
		gotoxy(74, i);
		printf("#");
	}
}


void preGame() {
	int numoffruitsleft = 10, snakesize = 10;
	char c = 0;
	Point *snake, fruit;

	//creating an dynamic array of points for the snake
	snake = (Point *)malloc(snakesize * sizeof(Point));
	if (snake == NULL)
		exit(1);

	//inserting a fruit on the board
	fruitInserting(snake, &numoffruitsleft, &snakesize, &fruit);

	//waiting for the player to press any arrow
	do {
		if (_kbhit())
			c = getKey();
	}
	while (c != UP && c != DOWN && c != LEFT && c != RIGHT && c != ESC);


	if (c != ESC)
		playingTime(snake, &c, &snakesize, &numoffruitsleft, &fruit);
	else
		cleanscreen();
}


void playingTime(Point snake[], char* c, int* snakesize, int* numoffruitsleft, Point* fruit) {
	int cellscreated = 1, i, x = middlecol, y = middleline;
	bool selfbumping = false, wallbumping = false;

	//initializing first cell
	snake[0].x = middlecol;
	snake[0].y = middleline;
	gotoxy(snake[0].x, snake[0].y);
	printf("@");

	while (((*c) != ESC) &&  (*numoffruitsleft != 0) && (selfbumping == false) && (wallbumping == false)) {

		// sleeps for a second before it continues to the next command
		Sleep(1000);

		if (_kbhit()) // if any key was hit
			(*c) = getKey();  // change direction

		switch ((*c)) {
			case RIGHT:
				x++;
				break;
			case DOWN:
				y++;
				break;
			case LEFT:
				x--;
				break;
			case UP:
				y--;
				break;
			case ESC:
				cleanscreen();
				break;
		}
		gotoxy(x, y);
		printf("@");

		if (cellscreated < ((*snakesize)))//if the snake still not whole
			cellscreated++;
		else if (cellscreated == ((*snakesize))) {//if the snake finished growing, erase the last cell in every iteration in order to make it "move"
			gotoxy(snake[cellscreated - 1].x, snake[cellscreated - 1].y);
			printf(" ");
		}

		//transferring cell to the cell after it
		for (i = (cellscreated) - 1; i >= 1; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}

		(snake[0].x) = x;
		(snake[0].y) = y;

		selfbumping = selfbump(snake, cellscreated);
		wallbumping = wallbump(snake);

		if (fruitEating(snake, fruit, numoffruitsleft)==true) {
			if (cellscreated == ((*snakesize))) {
				snake = reallocation(snake, snakesize);
				gotoxy(snake[(*snakesize) - 1].x, snake[(*snakesize) - 1].y);
				printf("@");
				cellscreated++;
			}
			fruitInserting(snake, numoffruitsleft, snakesize, fruit);
		}
	}

	cleanscreen();
	if (selfbumping==true || wallbumping==true)
		printf("Game over!\n");
	else if ((*numoffruitsleft == 0))
		printf("Congrats! You won:)\n");
	free(snake);
}


void fruitInserting(Point snake[], int* numoffruitsleft, int* snakesize, Point* fruit) {
	int fruitrow, fruitcol;
	bool touchingedges = false, touchingsnake = false;

	srand((int)time(NULL));

	do{
		fruitrow = rand() % (numofrows - 1);//random row location
		fruitcol = rand() % (numofcols - 1);//random column location
		touchingedges = fruittouchedges(fruitrow, fruitcol);
		if ((*numoffruitsleft) < 10) //if the game started
			touchingsnake = fruittouchsnake(snake, fruitrow, fruitcol, snakesize);
	}
	while (touchingedges == true || touchingsnake == true);

	(*fruit).x = fruitcol;
	(*fruit).y = fruitrow;
	gotoxy(fruitcol, fruitrow);
	printf("$");
}


bool fruittouchedges(int fruitrow, int fruitcol) {//true if the fruit is inserted in the edges
	if ((fruitrow == 0 || fruitrow == 24) || (fruitcol == 0 || fruitcol == 74))
		return true;
	else
		return false;
}

bool fruittouchsnake(Point snake[], int fruitrow, int fruitcol, int* snakesize) {//true if the fruit is inserted on the snake
	int i;
	for (i = 0; i < (*snakesize); i++) {//making sure not to insert a fruit inside the snake
		if ((snake[i].x == fruitcol) && (snake[i].y == fruitrow))
			return true;
	}
	return false;
}

bool fruitEating(Point snake[], Point* fruit, int* numoffruitsleft){
	if (snake[0].x == (*fruit).x && snake[0].y == (*fruit).y) {
		(*numoffruitsleft)--;
		return true;
	}
	else
		return false;
}

Point* reallocation(Point* snake, int* size) {
	int i, newSize=(*size)+1;
	Point* newsnake = (Point *)malloc(newSize * sizeof(Point));
	if (newsnake == NULL)
		exit(1);

	for (i = 0; i < (*size); i++) {
		newsnake[i] = snake[i];
	}

	//putting new last cell right after the others
	if (snake[(*size) - 1].x == snake[(*size) - 2].x) {
		newsnake[newSize-1].x = snake[(*size) - 1].x;
		newsnake[newSize-1].y = snake[(*size) - 1].y;
	}
	else if (snake[(*size) - 1].y == snake[(*size) - 2].y) {
		newsnake[newSize-1].x = snake[(*size) - 1].x;
		newsnake[newSize-1].y = snake[(*size) - 1].y;
	}

	*size = newSize;
	free(snake);
	return newsnake;
}


bool selfbump(Point snake[], int snakesize) {
	int i;
	for (i = 1; i < snakesize; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			return true;
		}
	}
	return false;
}


bool wallbump(Point snake[]) {
	if ((snake[0].y == 0 || snake[0].y == 24) || (snake[0].x == 0 || snake[0].x == 74)){//if the snake touched one of the edges
		return true;
	}
	return false;
}


// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape...
int getKey(void) {
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}


void cleanscreen(){
	int x, y;
	for (x = 0; x < 75; x++) {
		for (y = 0; y < 25; y++) {
			gotoxy(x, y);
			printf(" ");
		}
	}
	gotoxy(0, 0);
}
