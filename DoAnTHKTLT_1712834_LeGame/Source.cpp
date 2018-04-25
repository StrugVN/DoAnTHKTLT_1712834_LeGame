#include<iostream>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>

using namespace std;

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

struct Point {
	int x;
	int y;
};

Point Player;
Point *Finished = NULL;
int win = 0;
bool End = false;

void Goto(int x, int y) {
	COORD vt;
	vt.X = x;
	vt.Y = y;
	SetConsoleCursorPosition(consoleHandle, vt);
}

void FixedWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX)& ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void DrawBox() {
	SetConsoleTextAttribute(consoleHandle, (0 << 4) | 12);

	for (int i = 0; i < 24; i++) {
		Goto(60, i);
		cout << "|";
	}
	Goto(0, 24);

	SetConsoleTextAttribute(consoleHandle, (0 << 4) | 10);
	for (int i = 3; i < 25; i += 4) {
		Goto(0, i);
		for (int i = 0; i < 60; i++)
			cout << "-";
	}


	SetConsoleTextAttribute(consoleHandle, (0 << 4) | 15);
}

void DrawPlayer() {
	SetConsoleTextAttribute(consoleHandle, (0 << 4) | 7);
	Goto(Player.x - 1, Player.y - 1);
	cout << " O/";
	Goto(Player.x - 1, Player.y);
	cout << "/| ";
	Goto(Player.x - 1, Player.y + 1);
	cout << "/ \\";
	SetConsoleTextAttribute(consoleHandle, (0 << 4) | 15);
}

void ClearPlayer() {
	Goto(Player.x - 1, Player.y - 1);
	cout << "   ";
	Goto(Player.x - 1, Player.y);
	cout << "   ";
	Goto(Player.x - 1, Player.y + 1);
	cout << "   ";
}

bool Collide(Point a, Point b) {
	return (a.x == b.x && a.y == b.y);
}

bool check() {
	for (int i = 0; i < win; i++) {
		Point left, right;
		left.y = Finished[i].y;
		right.y = Finished[i].y;
		left.x = Finished[i].x - 1;
		right.x = Finished[i].x + 1;
		if (Collide(Player, left) || Collide(Player, right) || Collide(Player, Finished[i]))
			return false;
	}

	return true;
}

void _Move() {
	if (_kbhit())
		switch (_getch()) {
		case 'a':
		case 'A':
			if (Player.x - 1 > 0) {
				ClearPlayer();
				Player.x--;
				DrawPlayer();
			}
			break;
		case 'd':
		case 'D':
			if (Player.x - 1 < 57) {
				ClearPlayer();
				Player.x++;
				DrawPlayer();
			}
			break;
		case 'w':
		case 'W':
			ClearPlayer();
			Player.y -= 4;
			DrawPlayer();
			break;
		case 's':
		case 'S':
			if (Player.y + 4 <= 21) {
				ClearPlayer();
				Player.y += 4;
				DrawPlayer();
			}
			break;
		}
}

void Setup() {
	Player.y = 21;
	Player.x = 30;
	DrawBox();
	DrawPlayer();
}

void LvUp() {
	win++;
	Finished = (Point*)realloc(Finished, sizeof(Point)*win);
	Finished[win - 1] = Player;
	if (win >= 3)
		End = !End;
	Player.x = 29;
	Player.y = 21;
	DrawPlayer();
}

void Run() {
	while (!End) {
		_Move();
		if (Player.y == 1)
			LvUp();
		if (!check())
			break;
	}
}


int main() {
	FixedWindow();
	Setup();
	Run();

	_getche();
	return 0;
}

/*
___
__/___\__
|_@____@__\




0/
/|
/ \


*/