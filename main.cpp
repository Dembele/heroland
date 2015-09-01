#include <iostream>
#include <sys/ioctl.h>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <cstring>
#include <string>
#include <sstream>
#include "headers/defines.h"
#include "headers/getch.h"




using namespace std;

struct winsize w;

struct character 
{
int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp;
char name[100], area[101][101];
} char0, char1, char2;

int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp;
char name[100], area[101][101];

int ask, slot, temp;
int height, width;

int randomizer(int i)
{
	int token;
	return token = rand() % i + 1;
}

#include "headers/save-load.h"
#include "headers/mapcreate.h"
#include "headers/hello.h"
#include "headers/newgame.h"



void levelup()
{
	if(slot==0)
	{
		char0.level+=1;
		char0.nxp=(char0.nxp*120)/100;
		char0.stats+=3;
	}
	if(slot==1)
	{
		char1.level+=1;
		char1.nxp=(char1.nxp*120)/100;
		char1.stats+=3;
	}
	if(slot==2)
	{
		char2.level+=1;
		char2.nxp=(char2.nxp*120)/100;
		char2.stats+=3;
	}
}

void Map_Show(){	
	temp = area[y][x];
	area[y][x] = 'X';
	if (x > 14 && y > 7 && x < 86 && y < 93){							//центр
		for (int i = y + 7; i != y - 7; i--) {							// i = вверх/вниз у
			for (int j = x - 15; j != x + 15; j++) {					// j = вправо/влево х
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y <= 7){											//нижний левый угол
		for (int i = 13; i >= 0; i--) {
			for (int j = 0; j <= 30; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y > 7 && y < 93){								// центр слева
		for (int i = y+7; i != y-7; i--) {
			for (int j = 0; j != 30; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y >= 93){										// верх слева угол
		for (int i = 100; i != 86; i--) {
			for (int j = 0; j != 30; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x > 14 && y >= 93 && x<86){								//верх центр
		for (int i = 100; i != 86; i--) {
			for (int j = x - 15; j != x + 15; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y >= 93){											//верх справа угол
		for (int i = 100; i != 86; i--) {
			for (int j = 70; j <= 100; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y < 93 && y>7){									//центр справа
		for (int i = y+7; i != y-7; i--) {
			for (int j = 70; j <= 100; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y <= 7){											//низ справа угол
		for (int i = 13; i >= 0; i--) {
			for (int j = 70; j <= 100; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x > 14 && y <= 7 && x<86){											//низ центр
		for (int i = 13; i >= 0; i--) {
			for (int j = x-15; j != x+15; j++) {
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	area[y][x] = temp;
}

int main()
{
	cout<<"Разверните терминал, это необходимо для корректного отображения игры. Нажмите любую клавишу для продолжения...";
	getch();
	ioctl(0, TIOCGWINSZ, &w);
	height=w.ws_col; //40
	width=w.ws_row; //168
	if(height<70 || width<20)
	{
		cout<<"слишком мало места :(";
		return 1;
	}
	srand(time(NULL));
	Hello();
	if(slot==0)
	{
		NewGame();
		x=randomizer(100);
		y=randomizer(100);
		max_hp=vitality*2+1;
		hp=max_hp;
		MapCreate();
		SaveGame();
	}
	if(slot==1)Load_menu();
	while(hp>0)
	{
		printf("\033[2J\033[1;0H");
		cout<<"x: "<<x<<" y: "<<y<<endl;
		Map_Show();
		getch();
	}
    return 0;

}

