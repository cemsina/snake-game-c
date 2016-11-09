#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_X 15
#define MAX_Y 15
#pragma region Defining Types
typedef int bool;
enum boolEnum { false, true };
typedef enum boolEnum bool;
struct Location_s
{
	int x;
	int y;
};
typedef struct Location_s Location;
enum DirectionEnum {
	Up, Down, Left, Right
};
typedef enum DirectionEnum Direction;
struct Snake_s
{
	Direction SnakeDirection;
	Location Head;
	Location Tail[MAX_X * MAX_Y];
	int Length;
};
typedef struct Snake_s Snake;

#pragma endregion

#pragma region Global Properties
bool isDead;
Snake MainSnake;
Location t;
Location ActiveFood;
int Speed;
#pragma endregion
#pragma region User Interface
void VerticalWall() {
	printf("+");
	for (int i = 0; i < MAX_X; i++) {
		printf("-");
	}
	printf("+\n");
}
void ShowUI_Row(int y) {
	printf("|");
	for (int x = 0; x < MAX_X; x++) {
		if (MainSnake.Head.x == x && MainSnake.Head.y == y) {
			printf("@"); continue;
		}
		bool isEmpty = true;
		if (ActiveFood.x == x && ActiveFood.y == y) {
			printf("x"); isEmpty = false; continue;
		}
		for (int i = 0; i < MainSnake.Length - 1; i++) {
			if (MainSnake.Tail[i].x == x && MainSnake.Tail[i].y == y) {
				printf("o"); isEmpty = false; break;
			}
		}
		if (isEmpty == true) printf(" ");
	}
	printf("|\n");
}
void ShowUI() {
	system("cls");
	VerticalWall();
	for (int y = 0; y < MAX_Y; y++) {
		ShowUI_Row(y);
	}
	VerticalWall();
}
#pragma endregion
#pragma region Game Functions
bool CheckLocationIfEmpty(Location loc) {
	if (MainSnake.Head.x == loc.x && MainSnake.Head.y == loc.y) return false;
	for (int i = 0; i < MainSnake.Length - 1; i++) {
		if (MainSnake.Tail[i].x == loc.x && MainSnake.Tail[i].y == loc.y) return false;
	}
	return true;
}
Location FindNearestEmptyLocation(Location loc) {
	Location emptyLoc;
	int ScanX_Start = loc.x;
	int ScanX_End = loc.x;
	int ScanY_Start = loc.y;
	int ScanY_End = loc.y;
	if (MainSnake.Length == MAX_X * MAX_Y) isDead = true;
	while (1) {
		ScanX_Start--;
		ScanX_End++;
		ScanY_Start--;
		ScanY_End++;
		if (ScanX_Start < 0) ScanX_Start = 0;
		if (ScanY_Start < 0) ScanY_Start = 0;
		if (ScanX_End > MAX_X) ScanX_End = MAX_X;
		if (ScanY_End > MAX_Y) ScanY_End = MAX_Y;
		for (int x = ScanX_Start; x <= ScanX_End; x++) {
			for (int y = ScanY_Start; y <= ScanY_End; y++) {
				emptyLoc.x = x; emptyLoc.y = y;
				if (CheckLocationIfEmpty(emptyLoc) == true) {
					return emptyLoc;
				}
			}
		}
	}
}
void CreateRandomFood() {
	ActiveFood.x = rand() % MAX_X;
	ActiveFood.y = rand() % MAX_Y;
	bool isEmpty = false;
	if (CheckLocationIfEmpty(ActiveFood) == false) {
		ActiveFood = FindNearestEmptyLocation(ActiveFood);
	}
}
void NewGame() {
	MainSnake.SnakeDirection = Up;
	isDead = false;
	MainSnake.Head.x = 5;
	MainSnake.Head.y = 5;
	MainSnake.Length = 4;
	MainSnake.Tail[0].x = 5; MainSnake.Tail[0].y = 6;
	MainSnake.Tail[1].x = 5; MainSnake.Tail[1].y = 7;
	MainSnake.Tail[2].x = 5; MainSnake.Tail[2].y = 8;
	CreateRandomFood();
}

void GameOver() {
	printf("\n=============================\n");
	printf("=         Game Over !       =\n");
	printf("=============================\n");
	printf("       Final Length : %d     \n", MainSnake.Length);
}
void CheckIfCrashedByWall() {
	if (isDead == true) 
		return;
	if (MainSnake.Head.x >= MAX_X || MainSnake.Head.x < 0 || MainSnake.Head.y >= MAX_Y || MainSnake.Head.y < 0) {
		isDead = true;
	}
}
void CheckIfCrashedByTail() {
	if (isDead == true) 
		return;
	for (int i = 0; i < MainSnake.Length-1; i++) {
		if (MainSnake.Head.x == MainSnake.Tail[i].x && MainSnake.Head.y == MainSnake.Tail[i].y) {
			isDead = true; break;
		}
	}
}
void SetTail(Location zeroTailBackup) {
	for (int i = MainSnake.Length-2; i > 1; i--) {
		MainSnake.Tail[i].x = MainSnake.Tail[i-1].x;
		MainSnake.Tail[i].y = MainSnake.Tail[i-1].y;
	}
	MainSnake.Tail[1].x = zeroTailBackup.x;
	MainSnake.Tail[1].y = zeroTailBackup.y;
}

void AddNewLengthToSnake() {
	if (MainSnake.Length + 1 == MAX_X * MAX_Y) { isDead = true; MainSnake.Length++; return; }
	Location a = MainSnake.Tail[MainSnake.Length - 2];
	Location b = MainSnake.Tail[MainSnake.Length - 3];
	int DirX = b.x - a.x;
	int DirY = b.y - a.y;
	switch (DirX) {
	case 0:
		MainSnake.Tail[MainSnake.Length-1].x = a.x;
		break;
	case 1:
		MainSnake.Tail[MainSnake.Length-1].x = a.x - 1;
		break;
	case -1:
		MainSnake.Tail[MainSnake.Length-1].x = a.x + 1;
		break;
	default:
		isDead = true;
		printf("OMG you found a bug. How it's posibble\n");
	}
	switch (DirY) {
	case 0:
		MainSnake.Tail[MainSnake.Length-1].y = a.y;
		break;
	case 1:
		MainSnake.Tail[MainSnake.Length-1].y = a.y - 1;
		break;
	case -1:
		MainSnake.Tail[MainSnake.Length-1].y = a.y + 1;
		break;
	default:
		isDead = true;
		printf("OMG you found a bug. How it's posibble\n");
	}
	MainSnake.Length++;
}
void FoodHasBeenEaten() {
	AddNewLengthToSnake();
	CreateRandomFood();
}
void Move() {
	Location zero;
	zero.x = MainSnake.Tail[0].x;
	zero.y = MainSnake.Tail[0].y;
	MainSnake.Tail[0].x = MainSnake.Head.x;
	MainSnake.Tail[0].y = MainSnake.Head.y;
	switch (MainSnake.SnakeDirection) {
	case Up:
		MainSnake.Head.y -= 1;
		break;
	case Down:
		MainSnake.Head.y += 1;
		break;
	case Left:
		MainSnake.Head.x -= 1;
		break;
	case Right:
		MainSnake.Head.x += 1;
		break;
	}
	SetTail(zero);
	CheckIfCrashedByWall();
	CheckIfCrashedByTail();
	if (MainSnake.Head.x == ActiveFood.x && MainSnake.Head.y == ActiveFood.y) FoodHasBeenEaten();
}

void KeyPressHandler() {
	if (_kbhit()) {
		_getch();
		char key = _getch();
		switch (key)
		{
		case 77: //right
			if (MainSnake.SnakeDirection != Left) MainSnake.SnakeDirection = Right;
			break;
		case 75: //left
			if(MainSnake.SnakeDirection != Right) MainSnake.SnakeDirection = Left;
			break;
		case 72: //up
			if (MainSnake.SnakeDirection != Down) MainSnake.SnakeDirection = Up;
			break;
		case 80: //down
			if (MainSnake.SnakeDirection != Up) MainSnake.SnakeDirection = Down;
			break;
		default:
			break;
		}
	}
}
#pragma endregion
void GameHandler() {
	NewGame();
	ShowUI();
	while (isDead == false) {
		Sleep(Speed);
		KeyPressHandler();
		Move();
		ShowUI();
	}
	GameOver();
	getchar();
}
void main() {
	while (true) {
		printf("Set speed value for new game (1 - 100)\nSpeed : \n");
		int a;
		scanf_s("%d", &a);
		if (a > 100) a = 100;
		if (a < 1) a = 1;
		Speed = (int) 1100 - a * 10;
		GameHandler();
	}
	
}