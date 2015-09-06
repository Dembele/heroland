/* TODO list:
- enemies
- battle
- exp system
- inventory
- equiptable items
- special attacks
- ranged attacks
- magic
- drop
- craft
*/
#include <iostream>
#include <sys/ioctl.h>
#include <cstdio>
#include <iomanip>
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
int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp, attack, armour;
char name[100], area[101][101];
} char0, char1, char2;

int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp, attack, armour, armreduction;
char name[100], area[101][101];

int slot, temp, done, height, width, usedwidth, usedheight, saveslot;

int randomizer(int i)
{
	int token;
	return token = rand() % i + 1;
}

string ToStr(int number)
{
	string result;
	ostringstream convert;
	convert << number;
	result = convert.str();
	return result;
}

#include "headers/save-load.h"
#include "headers/mapcreate.h"
#include "headers/hello.h"
#include "headers/newgame.h"
#include "headers/interface.h"
#include "headers/actions.h"

void levelup()
{
	level+=1;
	nxp=(nxp*120)/100;
	stats+=3;
	max_hp += 5;
	hp=max_hp;
}

int main()
{
	setlocale(LC_CTYPE, "");
	cout<<"Разверните терминал и не изменяйте его размеры после этого, это необходимо для корректного отображения игры. Нажмите любую клавишу для продолжения...";
	getch();
	ioctl(0, TIOCGWINSZ, &w);
	height=w.ws_row; //40
	width=w.ws_col; //168
	if(height<20 || width<70)
	{
		cout<<endl<<"слишком мало места :("<<endl;
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
		saveslot=slot;
	}
	if(slot==1)Load_menu();
	while(hp>0)
	{
		printf("\033[2J\033[1;0H");
		cout<<"x: "<<x<<" y: "<<y<<endl;
		Map_Show();
		actions();
		DrawBotBar();
		switch(getch())
		{
			case 'w':
				if(y<99)y++;
			break;
			case 'a':
				if(x>0)x--;
			break;
			case 's':
				if(y>0)y--;
			break;
			case 'd':
				if(x<99)x++;
			break;
			case 'm':
				done=0;
				printf("\033[1;0H");
				for(int i=0;i<height-2;i++)cout<<endl;
				while(done!=1)
				{
					cout<<"\r";
					if(slot==0)YELLOW cout<<"Сохранить   ";	if(slot==0)RESET
					if(slot==1)YELLOW cout<<"Загрузка   "; 	if(slot==1)RESET
					if(slot==2)YELLOW cout<<"Отмена"; 		if(slot==2)RESET
					switch(getch())
					{
						case 'a':
							if(slot==0)slot=3;
							else slot--;
						break;
						case 'd':
							if(slot==3)slot=0;
							else slot++;
						break;
						case '\n':
							done=1;
						break;
					}
				}
				if (slot==0)SaveGame();
				if (slot==1)Load_menu();
			break;
			case 'l':
				Load_menu();
			break;
			case 'p':
				SaveGame();
			break;
		}
	}
    return 0;
}

