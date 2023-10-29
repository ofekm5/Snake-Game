#include "game.h"

int main() {
	int choice;
	do {
		mainMenu(&choice);
		if (choice != 0) {
			boardDrawing();
			preGame();

		}
	}
	while (choice != 0);

	return 0;
}
