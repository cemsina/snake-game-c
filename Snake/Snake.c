#include <stdio.h>
#include <Windows.h>
#define MAX_X 15
#define MAX_Y 15
#define Slowness 700
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
void NewGame() {
	MainSnake.SnakeDirection = Up;
	isDead = false;
	MainSnake.Head.x = 5;
	MainSnake.Head.y = 5;
	MainSnake.Length = 4;
	MainSnake.Tail[0].x = 5; MainSnake.Tail[0].y = 6;
	MainSnake.Tail[1].x = 5; MainSnake.Tail[1].y = 7;
	MainSnake.Tail[2].x = 5; MainSnake.Tail[2].y = 8;
}

void GameOver() {
	printf("\n=============================\n");
	printf("=         Game Over !       =\n");
	printf("=============================\n");
	printf("=     Final Length : %d     =\n", MainSnake.Length);
}
void CheckIfCrashedByWall() {
	if (isDead == true) return;
	if (MainSnake.Head.x > MAX_X || MainSnake.Head.x < 0 || MainSnake.Head.y > MAX_Y || MainSnake.Head.y < 0) {
		isDead = true;
	}
}
void CheckIfCrashedByTail() {
	if (isDead == true) return;
	for (int i = 0; i < MainSnake.Length; i++) {
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
	CheckIfCrashedByWall();
	CheckIfCrashedByTail();
	SetTail(zero);
}
#pragma endregion

void main() {
	NewGame();
	ShowUI();
	while (isDead == false) {
		Sleep(Slowness);
		Move();
		ShowUI();
	}
	GameOver();
	getchar();
}