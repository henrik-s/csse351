/*
This class is used as a game engine, keeping track of checker's placement,
whose turn it is, cheat mode, and rules etc.

Every cell in boardMatrix contains an integer for its square.
Codes:
	-1 = Not a valid square (32 of the total 64 squares)
	 0 = Empty valid square
	 1 = Standby regular red
	 2 = Selected regular red
	 3 = Standby red king
	 4 = Selected red king
	 5 = Standby regular black
	 6 = Selected black
	 7 = Standby black king
	 8 = Selected black king

**/
#ifndef __GAMEENGINE
#define __GAMEENGINE

#define DEBUG true

class GameEngine {
public:
	/*
	Initilize! The red color starts
	**/
	void init() {		
		topTurn = false;
		firstClick = true;
		cheatMode = false;
		gameOver = false;
		initBoard();
	}	

	/**
	Turn on or off cheat mode and print help text when cheat mode is
	enabled
	*/
	void cheat() {
		if (gameOver) // Do nothing
			return;
		if (!cheatMode) {
			cheatMode = true;
			printf("\n*** God-Mode is activated! *** \n");
			printf("In this mode, you are able to move\nyour checkers however you want.\n");
			printf("\nSimply select a checker of your color\nand click again on the box you want to\nmove it to.");
			printf(" Override your own\nchecker is not possible, instead it gets\nselected if you try to move\nanother checker on top of it.\n");
			printf("\nTo create a king, simply select a checker\nand press \'K\'. If you want to replace a king\nwith someone less important, select a king and\npress \'K\' again.\n");
		}
		else {
			cheatMode = false;
			printf("\n*** God-Mode is deactivated! *** \n");
		}
	}

	// Return true if cheat mode is on
	bool isCheatModeOn() {
		return cheatMode;
	}

	/**
	If cheat mode is enabled, convert a checker from regular to king or 
	reversed.
	*/
	void hailToTheKing() {
		if (!cheatMode)
			return;
		for(int y = 0; y<8; y++) {
			for(int x = 0; x<8; x++) {
				int tmp = boardMatrix[x][y];
				int newRole = tmp;
				switch (tmp) {
					case 2:			 // Red regular
						newRole = 4;
						printf("Red has crowned a new king!\n");
						break;
					case 4:			// Red king
						newRole = 2;
						printf("Red has slained a king!\n");
						break;
					case 6:			// Black regular
						newRole = 8;
						printf("Black has crowned a king!\n");
						break;
					case 8:			// Black king
						newRole = 6;
						printf("Black has slained a king!\n");
						break;
				}
				// Update board
				boardMatrix[x][y] = newRole;
			}
		}
	}

	/**
	Called from program3.cpp every time a mouse click event is triggered.
	If it is the first click of a player's turn, call perfromFirstClick with
	x and y coordinates. If it is the second click, call performSecondClick or 
	cheatEnvironment if cheat mode is on, using x and y coordinates. Lastly check
	if the game is over.
	*/
	void handleClick(int x, int y) {
		if (gameOver) 		// Do nothing
			return;
		if (x < 0 || y < 0) {
			helpText(0);
			return;
		}
		if (firstClick)
			performFirstClick(x,y);
		else {
			if (cheatMode)
				cheatEnvironment(x,y);
			else
				performSecondClick(x,y);
		}
		if (!gameOver)
			checkGameScore();
	}

	// Return a pointer to the board matrix
	int **getBoard() {
		return boardMatrix;
	}

private:
	bool cheatMode;
	bool gameOver;
	bool topTurn;
	bool firstClick;
	int x1, y1;	
	int** boardMatrix;

	/**
	In cheat environment, checkers can be moved all over the valid squares
	on the board. This method check if the new position is valid and updates
	corresponding values on the board.
	*/
	void cheatEnvironment(int x2, int y2) {
		// Select the already selected checker
		if(x2 == x1 && y2 == y1) {
			boardMatrix[x1][y1] -= 1; 		// Deselect
			firstClick = true;				// Indicate that no checker is selected
			return;
		}
		// Get the range for possible values for a team's checkers
		int teamLowerBound;
		int teamUpperBound;
		if (!topTurn) {		// Red team
			teamLowerBound = 0; teamUpperBound = 5;
		}
		else {				// Black team
			teamLowerBound = 4; teamUpperBound = 9;
		}
		// Check if the new move is overriding a opponent checker
		// If so, remove the opponent and move the selected checker 
		if (boardMatrix[x2][y2] > teamLowerBound && boardMatrix[x2][y2] < teamUpperBound) {
			boardMatrix[x1][y1] -= 1;
			boardMatrix[x2][y2] += 1;
			x1 = x2;
			y1 = y2;
		}
		// If the move is to a valid empty square
		else if (boardMatrix[x2][y2] > -1) {
			boardMatrix[x2][y2] = boardMatrix[x1][y1];
			boardMatrix[x1][y1] = 0;
			x1 = x2;
			y1 = y2;
		}
	}

	/**
	Upon a valid click/move, change the state of the 
	game to the next state.
	*/
	void click(int x, int y) {
		// First click, save the selected checker's position
		if (firstClick) {
			firstClick = false;
			x1 = x;
			y1 = y;
		}
		// End of turn
		else {
			firstClick = true;
			if (topTurn)
				topTurn = false;
			else
				topTurn = true;
		}
	}

	/**
	Check if the first click is on a player's own checker.
	If so, update the boardMatrix to indicate a selected checker
	*/
	void performFirstClick(int x, int y) {
		if(!topTurn && (boardMatrix[x][y] == 1 || boardMatrix[x][y] == 3))
			return firstClickUpdateBoard(x,y);
		else if (topTurn && (boardMatrix[x][y] == 5 || boardMatrix[x][y] == 7))			
			return firstClickUpdateBoard(x,y);

		helpText(1); 		// Print help text if the click is invalid
	}	

	/**
	Print a help text dependent on a game state.
	*/
	void helpText(int state) {
		if (topTurn)
			printf("Black's turn - ");
		else
			printf("Red's turn - ");
		if (state == 0)
			printf("You can NOT move to or select a checker from this position, for sure!\n");
		else if (state == 1) {
				printf("Select a red checker!\n");
		}
		else if (state == 2 && !cheatMode) 
				printf("You have a selected checker (%d, %d), try to move it to a valid square or click on it again to deselect!\n", x1, y1);
	}

	/**
	For a valid first click, update the board. Call click() to indicate 
	a change in game state.
	*/
	void firstClickUpdateBoard(int x, int y) {
		boardMatrix[x][y] += 1;
		click(x, y);
	}

	// Deselect a selected checker and update the game state.
	void undoMove(int x, int y) {
		boardMatrix[x][y] -= 1;
		firstClick = true;
	}

	/**
	On second click, check if it is an undo move (deselect)
	or a single diagonal move (move to empty valid square)
	or double diagonal move (jump over opponent checker)
	*/
	void performSecondClick(int x, int y) {
		if (x == x1 && y == y1) 			// Same coordinate as on first click
			return undoMove(x, y);
		if (trySingleMove(x,y))
			return;
		return tryDoubleMove(x, y);
	}

	/**
	Check if a move is to a valid empty adjacent square
	and if the checker is allowed to do it (king or not).
	If it is legal move, return true. Else false.
	*/
	bool trySingleMove(int x2, int y2) {
		int xDiff = x1 - x2;
		int yDiff = y1 - y2;
		if ((xDiff == 1 || xDiff == -1) && (yDiff == 1 || yDiff == -1)) {
			if (boardMatrix[x2][y2] == 0 && !tryToBeAKing(yDiff)) {
				updateBoard(x2, y2);		// Update board
				click(x2, y2);				// Indicate game state change
				return true;
			}
		}
		return false;
	}

	/**
	Check that the move is to a valid empty square and that an opponent checker
	is standing between the origin and new position of the moving checker.
	*/
	void tryDoubleMove(int x2, int y2) {
		int xDiff = x1 - x2;
		int yDiff = y1 - y2;
		if ((xDiff == 2 || xDiff == -2) && (yDiff == 2 || yDiff == -2)) {
			int interX = x1 - xDiff/2;
			int interY = y1 - yDiff/2;
			int oppColorChecker = topTurn ? 1 : 5;		// Find out the color for a opponent's regular standby checker
			int oppColorKing = topTurn ? 3 : 7; 		// Find out the color for a opponent's standby king
			if (boardMatrix[x2][y2] == 0 && (boardMatrix[interX][interY] == oppColorChecker || boardMatrix[interX][interY] == oppColorKing)) {
				if (tryToBeAKing(yDiff))		// If trying to be a king, abort
					return;
				boardMatrix[interX][interY] = 0; // Removed the destroyed checker
				updateBoard(x2, y2);			// Update old and new position for moving checker
				click(x2, y2);					// Indicate game state change
				return;
			}
		}
		helpText(2);		// Print help text if the second click was none of the three valid ones
	}

	/**
	Return true if a red/black regular checker is trying to move "backwards", otherwise false.
	*/
	bool tryToBeAKing(int yDiff) {
		if( (!topTurn && yDiff>0 && boardMatrix[x1][y1] != 4) || (topTurn && yDiff<0 && boardMatrix[x1][y1] != 8))
				return true;
		return false;
	}

	// Update board after a valid second click.
	// Check if the moved checker should turn into a king.
	void updateBoard(int x2, int y2) {		
		boardMatrix[x2][y2] = boardMatrix[x1][y1] -1;	// -1 to deselect moved checker
		boardMatrix[x1][y1] = 0;						// Old position
		if (!topTurn && y2 == 7) 
			boardMatrix[x2][y2] = 3;
		else if (topTurn && y2 == 0) 
			boardMatrix[x2][y2] = 7;
	}

	/**
	Initilize every cell on the board with -1, 0, 1, or 5.
	*/
	void initBoard() {
		boardMatrix = new int*[8];		
		for (int a = 0; a<8; a++) {
			boardMatrix[a] = new int[8];
		}
		for (int y = 0; y<8; y++) {
			for (int x = 0; x<8; x++) {
				int val = -1;
				if ((x % 2 != 0 && y % 2 == 0) || (x % 2 == 0 && y % 2 != 0)) {
					if (y < 3)
						val = 1;
					else if (y > 4)
						val = 5;
					else
						val = 0;
				}
				boardMatrix[x][y] = val;
			}
		}
	}

	/**
	Iterate through whole board and calculate the reamining
	checkers for both teams. If one team has 0, game is over!
	*/
	void checkGameScore() {
		int red = 0;
		int black = 0;
		int curr;
		for (int y = 0; y<8; y++) {
			for (int x = 0; x<8; x++) {
				curr = boardMatrix[x][y];
				if(curr > 0 && curr < 5)
					red++;
				else if (curr > 4)
					black++;
			}
		}
		gameOver = true;
		if (red == 0 && black == 0) {
			printf("\nA draw! Game over\n\n");
			return;
		}
		else if(red == 0) {
			printf("\nBlack wins! Game over\n\n");
			return;
		}
		else if(black == 0) {
			printf("\nRed wins! Game over\n\n");
			return;
		}		
		gameOver = false;
		}
};

#endif
