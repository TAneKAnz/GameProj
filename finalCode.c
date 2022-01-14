#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <fstream>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <string>
#include <conio.h>

int nScreenWidth = 120;
int nScreenHeight = 40;
int nMapWidth = 16;
int nMapHeight = 16;
int mune = 0;
int layer = 0;
int tab = 0;
int people;
#define nMenuWidth 40
#define nMenuHeight 40

float fPlayerX = 7.5;
float fPlayerY = 7.5;
float fGostX = 7.5;
float fGostY = 9.5;
float fPlayerA = 3.14;
float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;
float fSpeedP = 2.0f;
float fSpeedG = 1.0f;
float fElapsedTime;
bool bFollow = false;
bool state_game = false;
bool state_menu = true;
char state_ghost = ' ';
char state_map = '0';
char max_map = '1';
char ch = '.';
string name;
string list[10];

typedef struct _Ghost {
	float X;
	float Y;
	float S;
}Ghost, *PGhost;
Ghost ghostG, ghostH, ghostM, ghostI, ghostS, ghostX, ghostF, temp;
Ghost fall[5];

wstring map, map0, map1, map2, map3, map4, map5, map6, map7, map8, map9;

HANDLE wHnd;
COORD bufferSize = { nScreenWidth,nScreenHeight };
SMALL_RECT windowSize = { 0,0,nScreenWidth - 1,nScreenHeight - 1 };

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void menu()
{
	gotoxy(73, 10);
	printf("< Leader Board! >");
	gotoxy(25, 17);
	printf("*press 'w' to up");
	gotoxy(25, 18);
	printf("*press 's' to down");
	gotoxy(25, 19);
	printf("*press 'space bar' to enter");
	if (people == 0)
	{
		gotoxy(75, 12);
		printf("No History..");
	}
	for (int i = 0; i < people; i++)
	{
		gotoxy(75, 12 + i);
		cout << list[i];
	}
	gotoxy(25, 1);printf("####### #     # #######    #######");
	gotoxy(25, 2);printf("   #    #     # #          #        ####   ####    ##   #####  ######");
	gotoxy(25, 3);printf("   #    #     # #          #       #      #    #  #  #  #    # #");
	gotoxy(25, 4);printf("   #    ####### #####      #####    ####  #      #    # #    # #####");
	gotoxy(25, 5);printf("   #    #     # #          #            # #      ###### #####  #");
	gotoxy(25, 6);printf("   #    #     # #          #       #    # #    # #    # #      #");
	gotoxy(25, 7);printf("   #    #     # #######    #######  ####   ####  #    # #      ######");
	gotoxy(55, 38); printf("made by Thanakan Kanphairor 64010309 Computer Engineering KMITL");
	gotoxy(25, 10);
	printf("PLAY AGAIN");
	gotoxy(25, 12);
	printf("NEW");
	if (ch == '.')
	{
		gotoxy(23, 10);
		printf(">");
	}
	if (ch == ',')
	{
		gotoxy(73, 12 + tab);
		printf(">");
		gotoxy(23, 10);
		printf(" ");
	}
	if (_kbhit())
	{
		ch = _getch();
		if (ch == 'w')
		{
			if (layer == 0)
			{
				mune = 0;
			}
			if (layer == 1)
			{
				gotoxy(73, 12 + tab);
				printf(" ");
				tab--;
				if (tab < 0)
				{
					tab = people - 1;
				}
			}
		}
		if (ch == 's')
		{
			if (layer == 0)
			{
				mune = 1;
			}
			if (layer == 1)
			{
				gotoxy(73, 12 + tab);
				printf(" ");
				tab++;
				if (tab > people - 1)
				{
					tab = 0;
				}
			}
		}
		if (mune == 0 && layer == 0)
		{
			gotoxy(23, 10);
			printf(">");
			gotoxy(23, 12);
			printf(" ");
		}
		else if (mune == 1 && layer == 0)
		{
			gotoxy(23, 12);
			printf(">");
			gotoxy(23, 10);
			printf(" ");
		}
		if (layer == 0)
		{
			gotoxy(73, 12 + tab);
			printf(" ");
		}
		if (layer == 1)
		{
			gotoxy(23, 12);
			printf(" ");
			gotoxy(23, 10);
			printf(" ");
			gotoxy(73, 12 + tab);
			printf(">");
			if (ch == ' ')
			{
				for (int i = 0; i < (list[tab].length() - 2); i++)
				{
					name += list[tab][i];
				}
				max_map = list[tab][list[tab].length() - 1];
				state_menu = false;
				state_game = true;
			}
			if (ch == 'x')
			{
				layer = 0;
			}
		}
		if (mune == 0 && ch == ' ')
		{
			if (people != 0)
			{
				layer = 1;
				ch = ',';
			}
		}
		if (mune == 1 && ch == ' ')
		{
			gotoxy(40, 10);
			printf("Name = ");
			cin >> name;
			state_menu = false;
			state_game = true;
		}
	}
}
void check_hit();
void change_map();
void skill_ghost();
void follow_player();
bool compare(string a, string b);
int main()
{
	setcursor(0);
	string line;
	ifstream fin;
	fin.open("listPlayer.txt");
	while (fin) {
		getline(fin, line);
		list[people++] = line;
	}
	people--;

	while (state_menu)
	{
		menu();
	}

	setConsole(nScreenWidth, nScreenHeight);
	srand(time(NULL));

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
															 
	//wstring map;			     wstring map1;			       wstring map2;			     wstring map3;                 wstring map4;                 wstring map5;                 wstring map6;                 wstring map7;                 wstring map8;
	map0 += L"#######1########";  map1 += L"################";  map2 += L"################";  map3 += L"################";  map4 += L"################";  map5 += L"################";  map6 += L"################";  map7 += L"################";  map8 += L"################";  map9 += L"################";
	map0 += L"#######.########";  map1 += L"#..............#";  map2 += L"#..............#";  map3 += L"#......4#......#";  map4 += L"#.......#......#";  map5 += L"#..............#";  map6 += L"#.#....#.......#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..............#";
	map0 += L"#######.########";  map1 += L"#############..#";  map2 += L"#..##########..#";  map3 += L"#..######......#";  map4 += L"#.......#......#";  map5 += L"#######.#####..#";  map6 += L"#####..#.#####.#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..######......#";
	map0 += L"#######.########";  map1 += L"#...........#..#";  map2 += L"#..##########..#";  map3 += L"#..#...........#";  map4 += L"#.....###......#";  map5 += L"###.#########..#";  map6 += L"#......#.#...#.#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#...........#";
	map0 += L"#######.########";  map1 += L"#........#..#..#";  map2 += L"#..####........#";  map3 += L"#..#...........#";  map4 += L"#.....#........#";  map5 += L"#..............#";  map6 += L"#..#####.#.#...#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..######......#";
	map0 += L"#######.########";  map1 += L"#........#..#..#";  map2 += L"#..####.#####..#";  map3 += L"#..#...........#";  map4 += L"#.....#.#......#";  map5 += L"#..#####.#######";  map6 += L"#........#.#####";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#...........#";
	map0 += L"#######.########";  map1 += L"##.#######.##..#";  map2 += L"#..##.......#..#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"#..##.##########";  map6 += L"##########.....#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..######......#";
	map0 += L"1..............1";  map1 += L"#........#.....#";  map2 += L"#####.......#..#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"#..............#";  map6 += L"#........#####.#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..##...#......#";
	map0 += L"#######.########";  map1 += L"#..#.....#.##..#";  map2 += L"3..##.......#..#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"##########.##..#";  map6 += L"#.######.......#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#.#..#......#";
	map0 += L"#######.########";  map1 += L"#..#........#..#";  map2 += L"#..##.......#..#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"####.########..#";  map6 += L"#......#########";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#..#.#......#";
	map0 += L"#######.########";  map1 += L"#..#######..#..#";  map2 += L"#..####.#####..#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"#..............#";  map6 += L"######.........#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#...##......#";
	map0 += L"#######.########";  map1 += L"#.....#.....#..#";  map2 += L"#..####........#";  map3 += L"#..............#";  map4 += L"#.....#.#......#";  map5 += L"#..########.####";  map6 += L"#..............#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..######......#";
	map0 += L"#######.########";  map1 += L"#.....#........#";  map2 += L"#..##########..#";  map3 += L"#..............#";  map4 += L"#.....#5#......#";  map5 += L"#..#############";  map6 += L"#......###.....#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#.....#.....#";
	map0 += L"#######.########";  map1 += L"#####.##########";  map2 += L"#..##########..#";  map3 += L"#..............#";  map4 += L"#.....###......#";  map5 += L"#..##....##....#";  map6 += L"#......#7#.....#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..#.....#.....#";
	map0 += L"#######.########";  map1 += L"#..............2";  map2 += L"#..............#";  map3 += L"#..............#";  map4 += L"#..............#";  map5 += L"#.....##.....#.#";  map6 += L"#......###.....#";  map7 += L"#..............#";  map8 += L"#..............#";  map9 += L"#..######......#";
	map0 += L"#######1########";  map1 += L"################";  map2 += L"################";  map3 += L"################";  map4 += L"################";  map5 += L"##############6#";  map6 += L"################";  map7 += L"################";  map8 += L"################";  map9 += L"################";
	
	map = map0;
	map[7 * nMapWidth + 15] = max_map;
	if (max_map >= '3')
	{
		map[0 * nMapWidth + 7] = max_map - 1;
		map[15 * nMapWidth + 7] = max_map - 2;
	}

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (state_game)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		fElapsedTime = elapsedTime.count();

		skill_ghost();
		check_hit();

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (fSpeedP * 0.75f) * fElapsedTime;

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (fSpeedP * 0.75f) * fElapsedTime;

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * fSpeedP * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeedP * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * fSpeedP * fElapsedTime;;
				fPlayerY -= cosf(fPlayerA) * fSpeedP * fElapsedTime;;
			}
			change_map();
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeedP * fElapsedTime;;
			fPlayerY -= cosf(fPlayerA) * fSpeedP * fElapsedTime;;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeedP * fElapsedTime;;
				fPlayerY += cosf(fPlayerA) * fSpeedP * fElapsedTime;;
			}
			change_map();
		}
		if (GetAsyncKeyState((unsigned short)'X') & 0x8000)
		{
			string temple = name + ' ' + state_map;
			list[people++] = temple;
			for (int i = people - 1; i >= 1; i--)
			{
				if (compare(list[i - 1], list[i]))
				{
					string t = list[i];
					list[i] = list[i - 1];
					list[i - 1] = t;
				}
				else
				{
					break;
				}
			}
			ofstream fout;
			fout.open("listPlayer.txt");
			for (int i = 0; i < people; i++)
			{
				fout << list[i] << endl;
			}
			fout.close();
			state_game = false;
			state_menu = true;
		}

		for (int x = 0; x < nScreenWidth; x++)
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fStepSize = 0.1f;
			float fDistanceToWall = 0.0f;

			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						bHitWall = true;

						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;
					}
				}
			}

			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;
			else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < fDepth)				nShade = 0x2591;
			else											nShade = ' ';

			if (bBoundary)		nShade = ' ';

			for (int y = 0; y < nScreenHeight; y++)
			{

				if (y <= nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nShade;
				else
				{

					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'P';
		if (map.c_str()[(int)temp.X * nMapWidth + (int)temp.Y] != '#')
		{
			screen[((int)temp.X + 1) * nScreenWidth + (int)temp.Y] = state_ghost;
		}
		if (state_ghost == 'k')
		{
			for (int i = 0; i < 10; i += 3)
			{
				screen[((int)ghostI.X+i + 1) * nScreenWidth + (int)ghostI.Y] = 'i';
			}
		}
		if (state_ghost == 's')
		{
			screen[((int)temp.X + 1) * nScreenWidth + (int)temp.Y] = state_ghost;
		}

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	return 0;
}
void check_hit()
{
	if (((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY == ((int)temp.X + 1) * nScreenWidth + (int)temp.Y || map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#')
	{
		state_ghost = state_map;
	}
}
void change_map()
{
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '0')
	{
		map = map0;
		state_map = '0';
		fPlayerX = 7.5;
		fPlayerY = 7.5;
		fPlayerA = 0;
		bFollow = false;
		
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '1')
	{
		map = map1;
		state_map = '1';
		fPlayerX = 1.1;
		fPlayerY = 1.1;
		fPlayerA = 0;
		state_ghost = 'g';
		bFollow = false;
		ghostG.X = 14.5;
		ghostG.Y = 14.5;
		ghostG.S = 1;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '2')
	{
		map = map2;
		state_map = '2';
		fPlayerX = 6.9;
		fPlayerY = 1.5;
		fPlayerA = -1.57;
		state_ghost = 'g';
		bFollow = false;
		ghostG.X = 10;
		ghostG.Y = 14;
		ghostG.S = 1;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '3')
	{
		map = map3;
		state_map = '3';
		fPlayerX = 14;
		fPlayerY = 8;
		fPlayerA = -1.57;
		state_ghost = 'm';
		bFollow = false;
		ghostM.X = 4;
		ghostM.Y = 8;
		ghostM.S = 1;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '4')
	{
		map = map4;
		state_map = '4';
		fPlayerX = 1;
		fPlayerY = 7.5;
		fPlayerA = 1.57;
		state_ghost = 'm';
		bFollow = false;
		ghostM.X = 2;
		ghostM.Y = 15;
		ghostM.S = 1;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '5')
	{
		map = map5;
		state_map = '5';
		fPlayerX = 1.1;
		fPlayerY = 1.1;
		fPlayerA = 0;
		state_ghost = 'k';
		bFollow = false;
		ghostI.X = 1;
		ghostI.Y = 14;
		ghostI.S = 1.5;
		ghostG.X = 14.5;
		ghostG.Y = 14.5;
		ghostG.S = 0.8;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '6')
	{
		map = map6;
		state_map = '6';
		fPlayerX = 14;
		fPlayerY = 1.1;
		fPlayerA = -1.57;
		state_ghost = 's';
		bFollow = false;
		ghostS.X = 1.1;
		ghostS.Y = 1.1;
		ghostS.S = 0.4;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '7')
	{
		map = map7;
		state_map = '7';
		fPlayerX = (rand() % 14) + 1.5;
		fPlayerY = (rand() % 14) + 1.5;
		fPlayerA = rand() % 7;
		int x = 16 - fPlayerX;
		int y = 16 - fPlayerY;
		map[x * nMapWidth + y] = '8';
		state_ghost = 'x';
		bFollow = false;
		ghostX.X = (rand() % 14) + 1.5;
		ghostX.Y = (rand() % 14) + 1.5;
		ghostX.S = 0.8;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '8')
	{
		map = map8;
		int x = 0;
		int y = (rand() % 15) + 1;
		map[x * nMapWidth + y] = '9';
		state_map = '8';
		fPlayerX = 14.5;
		fPlayerA = -1.57;
		state_ghost = 'f';
		bFollow = false;
		ghostF.X = 1;
		ghostF.S = 0.4;
		for (int i = 0; i < 5; i++)
		{
			fall[i].X = (i * 3) + 1;
			fall[i].Y = (rand() % 14) + 1;
		}
		fPlayerY = fall[4].Y;
	}
	if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '9')
	{
		map = map9;
		state_map = '9';
		fPlayerX = 14.5;
	    fPlayerY = 1.5;
		fPlayerA = -1.57;
	}
}

void skill_ghost()
{
	if (state_ghost == state_map)
	{
		map[(int)fPlayerX * nMapWidth + (int)fPlayerY] = state_ghost;
	}
	if (state_ghost == 'g' || state_ghost == 'k')
	{
		if (((ghostG.X - fPlayerX < 2 && ghostG.X - fPlayerX >= 0) || (fPlayerX - ghostG.X < 2 && fPlayerX - ghostG.X >= 0)) && ((ghostG.Y - fPlayerY < 2 && ghostG.Y - fPlayerY >= 0) || (fPlayerY - ghostG.Y < 2 && fPlayerY - ghostG.Y >= 0)))
		{
			bFollow = true;
		}
		temp = { ghostG.X, ghostG.Y, ghostG.S };
		follow_player();
		ghostG = { temp.X, temp.Y, temp.S };
	}
	if (state_ghost == 'm')
	{
		bFollow = true;
		temp = { ghostM.X, ghostM.Y, ghostM.S };
		follow_player();
		ghostM = { temp.X, temp.Y, temp.S };
		if (map.c_str()[(int)temp.X * nMapWidth + (int)temp.Y] == '.')
		{
			map[(int)temp.X * nMapWidth + (int)temp.Y] = '#';
		}
	}
	if (state_ghost == 'i' || state_ghost == 'k')
	{
		for (int i = 0; i < 10; i += 3)
		{
			if (map.c_str()[((int)ghostI.X+i) * nMapWidth + (int)ghostI.Y] == '#')
			{
				map[((int)ghostI.X+i) * nMapWidth + (int)ghostI.Y] = '.';
			}
		}
		ghostI.Y -= ghostI.S * fElapsedTime;
		if (ghostI.Y <= 1.5 || ghostI.Y >= 14.5)
		{
			ghostI.S *= -1;
		}
		for (int i = 0; i < 10; i += 3)
		{
			if (map.c_str()[((int)ghostI.X + i) * nMapWidth + (int)ghostI.Y] == '.')
			{
				map[((int)ghostI.X + i) * nMapWidth + (int)ghostI.Y] = '#';
			}
		}
	}
	if (state_ghost == 's')
	{
		if (((ghostS.X - fPlayerX < 2 && ghostS.X - fPlayerX >= 0) || (fPlayerX - ghostS.X < 2 && fPlayerX - ghostS.X >= 0)) && ((ghostS.Y - fPlayerY < 2 && ghostS.Y - fPlayerY >= 0) || (fPlayerY - ghostS.Y < 2 && fPlayerY - ghostS.Y >= 0)))
		{
			bFollow = true;
		}
		if (map.c_str()[(int)ghostS.X * nMapWidth + (int)ghostS.Y] == '#')
		{
			map[(int)ghostS.X * nMapWidth + (int)ghostS.Y] = '.';
		}
		temp = { ghostS.X, ghostS.Y, ghostS.S };
		follow_player();
		ghostS = { temp.X, temp.Y, temp.S };
		if (map.c_str()[(int)temp.X * nMapWidth + (int)temp.Y] == '.')
		{
			map[(int)temp.X * nMapWidth + (int)temp.Y] = '#';
		}
	}
	if (state_ghost == 'x')
	{
		bFollow = true;
		temp = { ghostX.X, ghostX.Y, ghostX.S };
		follow_player();
		ghostX = { temp.X, temp.Y, temp.S };
		if (map.c_str()[(int)temp.X * nMapWidth + (int)temp.Y] == '.')
		{
			map[(int)temp.X * nMapWidth + (int)temp.Y] = state_map;
		}
	}
	if (state_ghost == 'f')
	{
		for (int i = 0; i < 5; i++)
		{
			for (int k = 1; k < 15; k++)
			{
				map[(int)fall[i].X * nMapWidth + k] = '.';
			}
			for (int l = 1; l < 15; l++)
			{
				map[15 * nMapWidth + l] = '#';
			}
			fall[i].X += ghostF.S * fElapsedTime;
			if (fall[i].X > 16)
			{
				fall[i].X = 1;
			}
			for (int j = 1; j < 16; j++)
			{
				if (j != fall[i].Y)
				{
					map[(int)fall[i].X * nMapWidth + j] = '#';
				}
			}
		}
	}
}

void follow_player()
{
	if (bFollow)
	{
		if (temp.X > fPlayerX)
		{
			if (temp.Y > fPlayerY)
			{
				temp.X -= temp.S * fElapsedTime;
				temp.Y -= temp.S * fElapsedTime;
			}
			else
			{
				temp.X -= temp.S * fElapsedTime;
				temp.Y += temp.S * fElapsedTime;
			}
		}
		else
		{
			if (temp.Y > fPlayerY)
			{
				temp.X += temp.S * fElapsedTime;
				temp.Y -= temp.S * fElapsedTime;
			}
			else
			{
				temp.X += temp.S * fElapsedTime;
				temp.Y += temp.S * fElapsedTime;
			}
		}
	}
}
bool compare(string a, string b)
{
	return a[a.length() - 1] < b[b.length() - 1];
}