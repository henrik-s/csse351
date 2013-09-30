#ifndef __GAMEENGINE
#define __GAMEENGINE

#define DEBUG true

class GameEngine {
public:
	void init() {		
		topTurn = false;
		firstClick = true;
		cheatMode = false;
		initBoard();
	}	

	void cheat() {
		if (!cheatMode) {
			cheatMode = true;
			printf("CheatMode is activated!\n");
		}
		else {
			cheatMode = false;
			printf("CheatMode is deactivated!\n");
		}
	}

	bool isCheatModeOn() {
		return cheatMode;
	}

	void hailToTheKing() {
		if (!cheatMode)
			return;
		for(int y = 0; y<8; y++) {
			for(int x = 0; x<8; x++) {
				int tmp = boardMatrix[x][y];
				int newRole = tmp;
				switch (tmp) {
					case 2:
						newRole = 4;
						printf("Red has crowned a new king!\n");
						break;
					case 4:
						newRole = 2;
						printf("Red has slained a king!\n");
						break;
					case 6:
						newRole = 8;
						printf("Black has crowned a king!\n");
						break;
					case 8:
						newRole = 6;
						printf("Black has slained a king!\n");
						break;
				}
				boardMatrix[x][y] = newRole;
			}
		}
	}

	void handleClick(int x, int y) {
		if (x < 0 || y < 0)
			return;
		if (firstClick)
			performFirstClick(x,y);
		else {
			if (cheatMode)
				cheatEnvironment(x,y);
			else
				performSecondClick(x,y);
		}
	}

	void printBoard() {	
		printf("*** Board status ***\n");	
		for (int y = 7; y>-1; y--) {
			for (int x = 0; x<8; x++) 
				printf("%i ", boardMatrix[x][y]);
			printf("\n");
		}
		printf("\n");
	}

	int **getBoard() {
		return boardMatrix;
	}

private:
	bool cheatMode;
	bool topTurn;
	bool firstClick;
	int x1, y1;	
	int** boardMatrix;

	void cheatEnvironment(int x2, int y2) {
		int teamLowerBound;
		int teamUpperBound;
		if (!topTurn) {
			teamLowerBound = 0; teamUpperBound = 5;
		}
		else {			
			teamLowerBound = 4; teamUpperBound = 9;
		}
		if (boardMatrix[x2][y2] > teamLowerBound && boardMatrix[x2][y2] < teamUpperBound) {
			boardMatrix[x1][y1] -= 1;
			boardMatrix[x2][y2] += 1;
			x1 = x2;
			y1 = y2;
		}
		else if (boardMatrix[x2][y2] > -1) {
			boardMatrix[x2][y2] = boardMatrix[x1][y1];
			boardMatrix[x1][y1] = 0;
			x1 = x2;
			y1 = y2;
		}
	}

	void click(int x, int y) {
		if (firstClick) {
			firstClick = false;
			x1 = x;
			y1 = y;
		}
		else {
			firstClick = true;
			if (topTurn)
				topTurn = false;
			else
				topTurn = true;
		}
	}

	void performFirstClick(int x, int y) {
		if(!topTurn && (boardMatrix[x][y] == 1 || boardMatrix[x][y] == 3))
			return firstClickUpdateBoard(x,y);
		else if (topTurn && (boardMatrix[x][y] == 5 || boardMatrix[x][y] == 7))			
			return firstClickUpdateBoard(x,y);
	}	

	void firstClickUpdateBoard(int x, int y) {
		boardMatrix[x][y] += 1;
		click(x, y);
	}

	void undoMove(int x, int y) {
		boardMatrix[x][y] -= 1;
		firstClick = true;
	}

	void performSecondClick(int x, int y) {
		if (x == x1 && y == y1)
			return undoMove(x, y);
		if (trySingleMove(x,y))
			return;
		return tryDoubleMove(x, y);
	}

	bool trySingleMove(int x2, int y2) {
		int xDiff = x1 - x2;
		int yDiff = y1 - y2;
		if ((xDiff == 1 || xDiff == -1) && (yDiff == 1 || yDiff == -1)) {
			if (boardMatrix[x2][y2] == 0 && !tryToBeAKing(yDiff)) {
				updateBoard(x2, y2);
				click(x2, y2);
				return true;
			}
		}
		return false;
	}

	void tryDoubleMove(int x2, int y2) {
		int xDiff = x1 - x2;
		int yDiff = y1 - y2;
		if ((xDiff == 2 || xDiff == -2) && (yDiff == 2 || yDiff == -2)) {
			int interX = x1 - xDiff/2;
			int interY = y1 - yDiff/2;
			int oppColorChecker = topTurn ? 1 : 5;
			int oppColorKing = topTurn ? 3 : 7; 
			if (boardMatrix[x2][y2] == 0 && (boardMatrix[interX][interY] == oppColorChecker || boardMatrix[interX][interY] == oppColorKing)) {
				if( tryToBeAKing(yDiff))
					return;
				boardMatrix[interX][interY] = 0;
				updateBoard(x2, y2);
				click(x2, y2);
				return;
			}
		}
	}

	bool tryToBeAKing(int yDiff) {
		if( (!topTurn && yDiff>0 && boardMatrix[x1][y1] != 4) || (topTurn && yDiff<0 && boardMatrix[x1][y1] != 8))
				return true;
		return false;
	}

	void updateBoard(int x2, int y2) {		
		boardMatrix[x2][y2] = boardMatrix[x1][y1] -1;
		boardMatrix[x1][y1] = 0;
		if (!topTurn && y2 == 7) 
			boardMatrix[x2][y2] = 3;
		else if (topTurn && y2 == 0) 
			boardMatrix[x2][y2] = 7;
	}

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

};

#endif
