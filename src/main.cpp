#include<iostream>
#include<time.h>
#include<cstdlib>
#include<SDL.h>
#include<SDL_image.h>
using namespace std;


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
const string TITLE = "GuessIt";
const int WIDTH = 1000;
const int HEIGHT = 600;
SDL_Texture *gameHumanImg, *gameCompImg, *startImg;
SDL_Texture *bigImg, *smallImg, *sameImg;
SDL_Texture *mark;
SDL_Texture *humanChooseNum;
SDL_Texture *humanWin,*compWin, *humanLose, *mode;
SDL_Texture *numImg[101];
SDL_Texture *countGuess[11];

SDL_Texture *loadTexture(string path);
bool init();
bool loadImage();
void close();
int humanChoice(int x, int y);
void chooseMode(int &choice);
void renderTexture(SDL_Texture *img, int startX, int startY, int width, int height);
void mainGame();
void startGame();
int calcHumanNum(int x, int y);
int calcNum(double x, double y, double startX, double startY, double endX,double endY, double stepX, double stepY);
void humanGuess(bool &quit, int &guess, double startX, double startY, double endX,double endY, double stepX, double stepY);
void showHumanGuess(int guess, int &numGuess);
bool continueGame(bool &quit);
void showComputerAnswer(int guess, int computerRand, bool &quit, bool &cont, int &numGuess, bool &lose);
int binarySearch(int left, int right);
void showComputerGuess(int res);
int humanClickResponse(int x, int y);
void humanResponse(int &left, int &right, int guess, int humanNum, bool &quit, bool &cont);
void choice1(bool &cont, bool &quit);
void choice2(bool &cont, bool &quit);

bool init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Init error\n";
		return 0;	
	} else {
		window = SDL_CreateWindow(TITLE.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
		if(window == NULL) {
			cout << "Create window error\n";
			return 0;
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL) {
				cout << "Render create error\n";
				return 0;
			}
			SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
		}
	}
	if(!loadImage()) {
		cout << "Can't load image, there's something missing\n";
		return 0;
	}
	return 1;
}

bool loadImage() {
	bool success = 1;
	gameHumanImg = loadTexture("images/HumaninGame.png");
	gameCompImg = loadTexture("images/CompGuess.png");
    startImg = loadTexture("images/startImg.png");
    bigImg = loadTexture("images/Big.png");
    smallImg = loadTexture("images/Small.png");
    sameImg = loadTexture("images/Same.png");
	mode = loadTexture("images/Mode.png");
	compWin = loadTexture("images/compWin.png");
	humanWin = loadTexture("images/Win.png");
	humanLose = loadTexture("images/Lose.png");
	humanChooseNum = loadTexture("images/humanChooseNum.png");
	if (gameHumanImg == NULL  || startImg == NULL || bigImg == NULL || smallImg == NULL
        || sameImg == NULL || compWin == NULL || humanWin == NULL || humanLose == NULL || gameCompImg == NULL
		) {
		cout << "load failed\n";
        success = 0;
    }
	for(int i = 1; i <= 100; i++) {
		numImg[i] = loadTexture("images/" + to_string(i) + ".bmp");
		if(numImg[i] == NULL) {
			success = 0;
			cout << "number failed\n";
			break;
		}
	}
	for(int i = 0; i <= 10; i++) {
		countGuess[i] = loadTexture("images/count" + to_string(i) + ".png");
		if(countGuess[i] == NULL) {
			success = 0;
			cout << "count guess image failed\n";
			break;
		}
	}
	return success;
}

void close() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(gameHumanImg);
	SDL_DestroyTexture(gameCompImg);
	SDL_DestroyTexture(startImg);
	SDL_DestroyTexture(mark);
	SDL_DestroyTexture(bigImg);
	SDL_DestroyTexture(sameImg);
	SDL_DestroyTexture(smallImg);
	for(int i = 1; i <= 100; i++) {
		SDL_DestroyTexture(numImg[i]);
	}
	for(int i = 0; i <= 10; i++) {
		SDL_DestroyTexture(countGuess[i]);
	}
	IMG_Quit();
	SDL_Quit();
}

int humanChoice(int x, int y) {
	int startX1 = 201;
	int startY1 = 210;
	int endX1 = 765;
	int endY1 = 311;
	int startX2 = 201;
	int startY2 = 384;
	int endX2 = 765;
	int endY2 = 482;
	if(x < startX1 || x > endX1 || y < startY1 
		|| y > endY2 || (y > endY1 && y < startY2)) {
			return 0;
	}
	if(x >= startX1 && x <= endX1 && y >= startY1 && y <= endY1) {
		return 1;
	} else {
		return 2;
	}
}

void chooseMode(int &choice) {
	SDL_RenderClear(renderer);
	renderTexture(mode,0,0,WIDTH,HEIGHT);
	SDL_RenderPresent(renderer);
	SDL_Event e;
	while(choice == 0) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				close();
				exit(0);
			} else if(e.type == SDL_MOUSEBUTTONDOWN) {
				// cout << e.button.x << " " << e.button.y << "\n";
				choice = humanChoice(e.button.x,e.button.y);
			}
		}
	}
	SDL_Delay(500);
}

SDL_Texture *loadTexture(string path) {
	SDL_Surface *image = IMG_Load(path.c_str());
	SDL_Texture *tmp = NULL;
	if(image == NULL) {
		cout << "Load image error\n";
	} else {
		tmp = SDL_CreateTextureFromSurface(renderer,image);
		SDL_FreeSurface(image);
		if(tmp == NULL) {
			cout << "Create texture error\n";
		}
	}
	return tmp;
}

void renderTexture(SDL_Texture *img, int startX, int startY, int width, int height) {
	SDL_Rect frame = {startX, startY, width, height};
	SDL_RenderCopy(renderer,img,NULL,&frame);
}

void mainGame(SDL_Texture *x) {
	SDL_RenderClear(renderer);
	renderTexture(x,0,0,WIDTH,HEIGHT);
	SDL_RenderPresent(renderer);
}

void startGame() {
	SDL_RenderClear(renderer);
	renderTexture(startImg,0,0,WIDTH,HEIGHT);
	SDL_RenderPresent(renderer);
	SDL_Delay(3000);
}

int calcNum(double x, double y, double startX, double startY, double endX,double endY, double stepX, double stepY) {
	if(x < startX || x > endX || y < startY || y > endY) {
		return 0;
	} 
	int col = (x - startX) / stepX + 1;
	int row = (y - startY) / stepY + 1;
	if(col > 20) {
		col = 20;
	}
	if(row > 10) {
		row = 10;
	}
	int res = (row - 1) * 10 + col;
	return res;
}

void humanGuess(bool &quit, int &guess, double startX, double startY, double endX,double endY, double stepX, double stepY) {
	SDL_Event e;
	while(guess <= 0 || guess > 100) {
		if(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = 1;
				return;
			} else if(e.type == SDL_MOUSEBUTTONDOWN) {
				guess = calcNum(e.button.x,e.button.y,startX,startY,endX,endY,stepX,stepY);
			}
		}
	}
}

void showHumanGuess(int guess, int &numGuess) {
	renderTexture(numImg[guess], 290, 252, 74, 61);
    SDL_RenderPresent(renderer);
	numGuess--;
	renderTexture(countGuess[numGuess],756,524,46,38);
}

bool continueGame(bool &quit) {
	SDL_Event e;
	int startX = 277;
	int startY = 374;
	int endX = 707;
	int endY = 500;
	while(1) {
		while(SDL_WaitEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = 1;
				return 0;
			} else if(e.type == SDL_MOUSEBUTTONDOWN) {
				int x = e.button.x;
				int y = e.button.y;
				if(x > startX && x < endX && y > startY && y < endY) {
					return 1;
				}
			}
		}
	}
	return 1;
}

void showComputerAnswer(int guess, int computerRand, bool &quit, bool &cont, int &numGuess, bool &lose) {
	if(guess > computerRand) {
		renderTexture(bigImg,190,39,260,122);
		SDL_RenderPresent(renderer);
	} else if(guess < computerRand) {
		renderTexture(smallImg,190,39,260,122);
		SDL_RenderPresent(renderer);
	} else {
		renderTexture(sameImg,190,39,260,122);
		SDL_RenderPresent(renderer);
		SDL_Delay(2000);
		SDL_RenderClear(renderer);
		renderTexture(humanWin,0,0,WIDTH,HEIGHT);
		SDL_RenderPresent(renderer);
		cont = continueGame(quit);
		return;
	}
	if(numGuess == 0) {
		lose = 1;
		renderTexture(countGuess[0],756,524,46,38);
		SDL_Delay(1000);
		SDL_RenderClear(renderer);
		renderTexture(humanLose,0,0,WIDTH,HEIGHT);
		SDL_RenderPresent(renderer);
		cont = continueGame(quit);
	}
}

int binarySearch(int left, int right) {
	return (left+right)/2;
}

void showComputerGuess(int res) {
	SDL_Delay(800);
	renderTexture(numImg[res], 240, 90, 250, 100);
    SDL_RenderPresent(renderer);
}

int humanClickResponse(int x, int y) {
	if(x < 23 || x > 948 || y < 413 || y > 568 || (x > 290 && x < 354)
		|| (x > 624 && x < 675)) {
		return -2;
	}
	if(x > 23 && x < 290 ) {
		return -1;
	}
	if(x > 354 && x < 624) {
		return 0;
	}
	return 1;
}

void humanResponse(int &left, int &right, int guess, int humanNum, bool &quit, bool &cont) {
	int result = -2;
	SDL_Event e;
	while(result == -2) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = 1;
				return;
			} else if(e.type == SDL_MOUSEBUTTONDOWN) {
				result = humanClickResponse(e.button.x,e.button.y);
			}
		}
	}
	if(guess == humanNum) {
		if(result != 0) {
			return;
		}
		SDL_RenderClear(renderer);
		renderTexture(compWin,0,0,WIDTH,HEIGHT);
		SDL_RenderPresent(renderer);
		cont = continueGame(quit);
		return;
	}
	if(guess < humanNum) {
		if(result != 1) {
			return;
		}
		left = guess;
	} else if(guess > humanNum) {
		if(result != -1) {
			return;
		}
		right = guess;
	}
}

void choice1(bool &cont, bool &quit) {
	srand(time(NULL));
	int numGuess = 10;
	bool Lose = 0;
	SDL_Event e;
	mainGame(gameHumanImg);
	int computerRand = rand() % 100 + 1;
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = 1;
				return;
			}
			int guess = 0;
			humanGuess(quit,guess,504,96,923,491,42,39.5);
			showHumanGuess(guess, numGuess);
			showComputerAnswer(guess,computerRand,quit,cont,numGuess,Lose);
		}
		if(cont == 1) {
			break;
		}
	}
}

void choice2(bool &cont, bool &quit) {
	int humanNum = 0;
	SDL_Event e;
	int left = 0, right = 100;
	mainGame(humanChooseNum);
	humanGuess(quit,humanNum,4,129,997,597,98,46.5);
	SDL_Delay(1000);
	mainGame(gameCompImg);
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				quit = 1;
				return;
			}
			int guess = binarySearch(left,right);
			showComputerGuess(guess);
			humanResponse(left,right,guess,humanNum,quit,cont);
		}
		if(cont == 1) {
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	if(init()) {
		SDL_Event e;
		startGame();
		bool cont = 0;
		do {
			int choice = 0;
			chooseMode(choice);
			bool quit = 0;
			cont = 0;
			if(choice == 1) {
				choice1(cont,quit);
			} else {
				choice2(cont,quit);
			}
			if(quit == 1) {
				break;
			}
		} while(cont == 1);
		close();
	}
	return 0;
}
