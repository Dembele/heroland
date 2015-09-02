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
int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp;
char name[100], area[101][101];
} char0, char1, char2;

int stats, hp, max_hp, strength, vitality, level, x, y, xp, nxp;
char name[100], area[101][101];

int ask, slot, temp;
int height, width, usedwidth, usedheight;

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
	level+=1;
	nxp=(char2.nxp*120)/100;
	stats+=3;
}

void Map_Show()
{	
	temp = area[y][x];
	area[y][x] = 'X';
	if (x > 14 && y > 7 && x < 86 && y < 93)
	{							//центр
		for (int i = y + 7; i != y - 7; i--)
		{
			cout.width(width-30);
			cout<<right;												// i = вверх/вниз у
			for (int j = x - 15; j != x + 15; j++)
			{					// j = вправо/влево х
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y <= 7)
	{											//нижний левый угол
		for (int i = 13; i >= 0; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 0; j <= 30; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y > 7 && y < 93)
	{								// центр слева
		for (int i = y+7; i != y-7; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 0; j != 30; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x <= 14 && y >= 93)
	{										// верх слева угол
		for (int i = 100; i != 86; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 0; j != 30; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x > 14 && y >= 93 && x<86)
	{								//верх центр
		for (int i = 100; i != 86; i--)
		{
			cout.width(width-30);
			cout<<right;		
			for (int j = x - 15; j != x + 15; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y >= 93){											//верх справа угол
		for (int i = 100; i != 86; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 70; j <= 100; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y < 93 && y>7)
	{									//центр справа
		for (int i = y+7; i != y-7; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 70; j <= 100; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x >= 86 && y <= 7)
	{											//низ справа угол
		for (int i = 13; i >= 0; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = 70; j <= 100; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	if (x > 14 && y <= 7 && x<86)
	{											//низ центр
		for (int i = 13; i >= 0; i--)
		{
			cout.width(width-30);
			cout<<right;	
			for (int j = x-15; j != x+15; j++)
			{
				cout << area[i][j];
			}
			cout << endl;
		}
	}
	area[y][x] = temp;
}

void SidePrint(wstring st)
{
	for (int i=0; i<(int)st.length(); i++)
	{
		if(usedwidth<width-16 && usedheight<16)
		{
			wcout<<st.at(i);
			width++;
		}
		if(usedwidth==width-16 && usedheight<16)
		{
			wcout<<endl;
			width=0;
		}
		
		if(usedheight>=16) wcout<<st.at(i);
	}
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
				switch(getch())
				{
					int done=0;
					while(done!=1)
					{
						case 'w':
							if(slot==0)slot=3;
							else slot++;
						break;
						case 's':
							if(slot==3)slot=0;
							else slot--;
						break;
						case 'd':
							
						break;
						case 's':
							done=1;
						break;
					}
				}
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

