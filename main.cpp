/* TODO list:
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
#include "defines.h"
#include "getch.h"

using namespace std;

struct winsize w;

void DrawBotBar();
void DrawTopBar();

struct character
{
int stats, hp, maxHp, strength, vitality, level, x, y, xp, nxp, attack, regeneration, armour;
int damageReduction;
char name[100], area[101][101];
} char0, char1, char2;

struct enemy
{
int stats[200], hp[200], maxHp[200], strenght[200], vitality[200], level[200], x[200], y[200], xpGain[200], attack[200], armour[200];
int damageReduction[200];
int mask[101][101];
} enemy;

int id, stats, hp, maxHp, strength, vitality, level, x, y, xp, nxp, attack, armour, regeneration;
int damageReduction;
char name[100], area[101][101], battleArea[20][20];

int slot, temp, done, height, width, usedwidth, usedheight, saveslot, day[1], prev;

bool nothingIsDone;

wstring ToWstr(int number)
{
    wostringstream convert;
    convert << number;
    const wstring s(convert.str());
    return s;
}

int Randomizer(int i)
{
    int token;
    return token = rand() % i + 1;
}

void StatsCount()
{
    attack              = strength * 2;
    damageReduction     = ((armour+100)/armour);
    maxHp               = vitality * 2;
    hp                  = maxHp;
}

// todo: check for whether already 200 enemies
void EnemyCreate()
{
    for(int i=0; i<200; i++)
    {
        if(enemy.level[id]==0)
        {
            if(level>7)enemy.level[id]  = rand()%14 + level - 6;
            else enemy.level[id]        = rand()%7 + level + 1;
            enemy.stats[id]             = level * 2;
            int temp                    = rand() % enemy.stats[id] + 1;
            enemy.vitality[id]          = temp + 1;
            enemy.strenght[id]          = enemy.stats[id] - temp + 1;
            enemy.maxHp[id]             = enemy.vitality[id] * 3;
            enemy.hp[id]                = enemy.maxHp[id];
            enemy.attack[id]            = enemy.strenght[id] * 3;
            enemy.xpGain[id]            = enemy.stats[id] + 4;
            enemy.armour[id]            = rand() % 100 - 20;
            enemy.damageReduction[id]   = enemy.armour[id]/(enemy.armour[id]+100);
            enemy.x[id]                 = rand() % 100;
            enemy.y[id]                 = rand() % 100;
            while(enemy.mask[enemy.x[id]][enemy.y[id]]>0)
            {
                enemy.x[id]                 = rand() % 100;
                enemy.y[id]                 = rand() % 100;
            }
            enemy.mask[enemy.x[id]][enemy.y[id]] = enemy.level[id];
            id++;
            break;
        }
        else id++;
    }
}

void MapShow()
{
    temp = area[y][x];
    area[y][x] = 'X';
    printf("\033[2;0H");
    if (x > 14 && y > 7 && x < 86 && y < 93)
    {							//центр
        for (int i = y + 7; i != y - 7; i--)
        {
            wcout.width(width-30);
            wcout<<right;												// i = вверх/вниз у
            for (int j = x - 15; j != x + 15; j++)
            {					// j = вправо/влево х
                    if (enemy.mask[i][j]>=level+7) RED;
                    if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                    if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                    wcout << area[i][j];
                    if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x <= 14 && y <= 7)
    {											//нижний левый угол
        for (int i = 13; i >= 0; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 0; j != 30; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x <= 14 && y > 7 && y < 93)
    {								// центр слева
        for (int i = y+7; i != y-7; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 0; j != 30; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x <= 14 && y >= 93)
    {										// верх слева угол
        for (int i = 100; i != 86; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 0; j != 30; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x > 14 && y >= 93 && x<86)
    {								//верх центр
        for (int i = 100; i != 86; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = x - 15; j != x + 15; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x >= 86 && y >= 93){											//верх справа угол
        for (int i = 100; i != 86; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x >= 86 && y < 93 && y>7)
    {									//центр справа
        for (int i = y+7; i != y-7; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x >= 86 && y <= 7)
    {											//низ справа угол
        for (int i = 13; i >= 0; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    if (x > 14 && y <= 7 && x<86)
    {											//низ центр
        for (int i = 13; i >= 0; i--)
        {
            wcout.width(width-30);
            wcout<<right;
            for (int j = x-15; j != x+15; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                wcout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            wcout << endl;
        }
    }
    area[y][x] = temp;
}

void SidePrint(wstring st)
{
    for (int i=0; i<(int)st.length(); i++)
    {
        if(usedwidth<width-32 && usedheight<32 && usedheight<14)
        {
            wcout<<st.at(i);
            usedwidth++;
        }
        if(usedwidth==width-32 && usedheight<32 && usedheight<14)
        {
            wcout<<endl;
            usedwidth=0;
            usedheight++;
        }

        if(usedheight>=14 && usedheight<height-3)
        {
            if (usedwidth!=width)
            {
                wcout<<st.at(i);
                usedwidth++;
            }
            if (usedwidth==width)
            {
                usedwidth=0;
                usedheight++;
            }
        }
        if(usedheight==height-3)
        {
            wcout<<L"Место на экране закончилось... нажмите любую клавишу для продолжения\033[2;0H"<<endl;
            getch();
            wcout<<L"\033[02J";
            DrawTopBar();
            MapShow();
            DrawBotBar();
            wcout<<L"\033[2;0H";
            usedheight=0; usedwidth=0;
        }
    }
}

void DrawBotBar()
{
    wstring placebottom = L"\033[";
    placebottom.append(ToWstr(height));
    placebottom.append(L";0H");
    wcout<<placebottom;
    wcout<<L"Уровень: "<<level<<L" Опыт: "<<xp<<L"/"<<nxp<<L" Здоровье: "<<hp;
    wcout<<L"/"<<maxHp << L" Атака: "<<attack<<L" Броня: " << damageReduction<<L"%";
}

void DrawTopBar()
{
    wcout<<L"\033[02J\033[1;0Hx: "<<x<<L" y: "<<y;
    switch(area[y][x])
    {
        case '@':
            wcout<<L" Вы входите в лес";
        break;
        case '.':
            wcout<<L" Вы входите в пустыню";
        break;
        case '*':
            wcout<<L" Вы входите в джунгли";
        break;
        case '^':
            wcout<<L" Вы поднимаетесь в горы";
        break;
        case '#':
            wcout<<L" Вы поднимаетесь на плато";
        break;
        case '!':
            wcout<<L" Вы входите в болото";
        break;
        case '%':
            wcout<<L" Вы поднимаетесь в холмы";
        break;
        case '~':
            wcout<<L" Вы выходите к водоему";
        break;
    }
    wcout<<L". День: " << day[0] << L". Ходы:" << 10-day[1] << endl;
}

void BattleMapCreate()
{
    int mainGround = (int) area[y][x];
    for(int i=0; i<20; i++)
        for (int j=0; j<20; j++)
            battleArea[i][j]=(char) mainGround;
    int otherGround = Randomizer(5);
    for(int i=0; i<otherGround; i++)
        battleArea[Randomizer(21)-1][Randomizer(21)-1]=Randomizer(5);
    for(int i=0; i<20; i++)
        for(int j=0; j<20; j++)
        {
            switch(battleArea[i][j])
            {
                case 1:
                    battleArea[i][j]='~';
                break;
                case 2:
                    battleArea[i][j]='@';
                break;
                case 3:
                    battleArea[i][j]='#';
                break;
                case 4:
                    battleArea[i][j]='$';
                break;
                case 5:
                    battleArea[i][j]='%';
                break;
            }
        }
}

void BattleMapShow()
{
    wcout.width(width-20);
    wcout<<right;
    for (int i=0; i<20; i++)
    {
        for(int j=0; j<20; j++)
        {
            wcout<<area[i][j];
        }
        wcout<<endl;
    }
}

void Battle()
{
    int xTemp=x, yTemp=y;
    BattleMapCreate();
    SidePrint(L"\033[2;0Hкарта создана");
    x=0; y=Randomizer(20);
    battleArea[y][x]='x';
    for(int i=0; i<200; i++)
        if(enemy.y[i]==y && enemy.x[i]==x)
        {
            id=i;
            break;
        }
    SidePrint(L"\033[2;0Hпротивник найден");
    wcout<<enemy.hp[id];
    while (hp>0 && enemy.hp[id]>0)
    {
       wcout<<L"\033[02J\033[1;0Hx: "<<x<<L" y: "<<y<<L" Здоровье: "<<hp;
       wcout<<L"/"<<maxHp << L" Атака: "<<attack<<L" Броня: " << damageReduction<<L"%";;
       BattleMapShow();

    }
    x=xTemp; y=yTemp;
}

void MapCreate()
{
    for(int i=0; i<=100; i++)
        for(int j=0; j<=100; j++)
        {
            int compare=Randomizer(100);
            if (i==0 && j==0) area[i][j]=Randomizer(7);
            if (i==0 && j>0 && compare>75) area[i][j]=Randomizer(7);
            if (i==0 && j>0 && compare<=75) area[i][j]=area[i][j-1];
            if (i>0 && j==0 && compare>75) area[i][j]=Randomizer(7);
            if (i>0 && j==0 && compare<=75) area[i][j]=area[i-1][j];
            if (i>0 && j>0 && area[i-1][j]==area[i][j-1]) area[i][j]=area[i-1][j];
            if (i>0 && j>0 && area[i-1][j]!=area[i][j-1])
            {
                if (compare<=40) area[i][j]=area[i-1][j];
                if (compare>40 && compare<80) area[i][j]=area[i][j-1];
                if (compare>=80) area[i][j]=Randomizer(7);
            }
        }
        for(int i=0; i<=100; i++)
            for(int j=0; j<=100; j++)
                if (Randomizer(100)>90) area[i][j]=8;

        for(int i=0; i<=100; i++)
            for(int j=0; j<=100; j++)
            {
                if(area[i][j]==1)area[i][j]='@'; //лес
                if(area[i][j]==2)area[i][j]='.'; //пустыня
                if(area[i][j]==3)area[i][j]='*'; //джунгли
                if(area[i][j]==4)area[i][j]='^'; //гора
                if(area[i][j]==5)area[i][j]='#'; //плато
                if(area[i][j]==6)area[i][j]='!'; //болото
                if(area[i][j]==7)area[i][j]='%'; //холм
                if(area[i][j]==8)area[i][j]='~'; //водоем
            }
}

void NewGame()
{
    wcout << L"\033[2J\033[1;0HВыберите слот для сохранения:" << endl;
    if(ifstream("save_0.dat").good()==1)
    {
        ifstream ifs0 ("save_0.dat");
        ifs0 >> char0.name;
        ifs0 >> char0.level;
    }
    else strcpy(char0.name,"Пусто");
    if(ifstream("save_1.dat").good()==1)
    {
        ifstream ifs1 ("save_1.dat");
        ifs1 >> char1.name;
        ifs1 >> char1.level;
    }
    else strcpy(char1.name,"Пусто");
    if(ifstream("save_2.dat").good()==1)
    {
        ifstream ifs2 ("save_2.dat");
        ifs2 >> char2.name;
        ifs2 >> char2.level;
    }
    else strcpy(char2.name,"Пусто");
    int done=0;
    while(done==0)
    {
        wprintf(L"\033[2J\033[1;0H");
        if (slot==0)YELLOW;
        wcout<<char0.name<<L" "<<char0.level<<L" уровень"<<endl;
        if (slot==0)RESET;
        if (slot==1)YELLOW;
        wcout<<char1.name<<L" "<<char1.level<<L" уровень"<<endl;
        if (slot==1)RESET;
        if (slot==2)YELLOW;
        wcout<<char2.name<<L" "<<char2.level<<L" уровень"<<endl;
        if (slot==2)RESET;
        switch(getch())
        {
            case 's':
                if(slot<2)slot++;
                else slot=0;
            break;
            case '\n':
                done=1;
            break;
            case 'w':
                if(slot>0)	slot--;
                else slot=2;
            break;
        }
    }
    wcout<<endl;
    wcout<<L"Введите имя персонажа: ";
    cin>>name;
    stats=15;
    int old_slot=slot;
    while (stats>=1)
    {
        wprintf(L"\033[2J\033[1;0H");
        if (slot==0) YELLOW;
        wcout<<L"Сила        : ";
        for(int i=0; i<strength; i++) wcout<< "*";
        wcout<< endl;
        if (slot==0) RESET;
        if (slot==1) YELLOW;
        wcout<<L"Выносливость: ";
        for(int i=0; i<vitality; i++) wcout<< "*";
        wcout<< endl;
        if (slot==1) RESET;
        wcout << L"Cвободные очки: ";
        for(int i=0; i<stats; i++) wcout<< "*";
        wcout<< endl;
        switch(getch())
        {
            case 'a':
                if(slot==0 && strength>0)
                {
                    strength--;
                    stats++;
                }
                if(slot==1 && vitality>0)
                {
                    vitality--;
                    stats++;
                }
            break;
            case 's':
                if(slot==0)slot++;
                else slot=0;
            break;
            case 'd':
                if(slot==0) strength++;
                if(slot==1) vitality++;
                stats--;
            break;
            case 'w':
                if(slot==1)	slot--;
                else slot=1;
            break;
        }
    }
    slot=old_slot;
    wprintf(L"\033[2J\033[1;0H");
        wcout<<name<< L", на старте у вас будет Сила: " << strength << L" Выносливость: " << vitality << endl;
        getch();
        level               = 1;
        nxp                 = 100;
        armour              = 16;
        StatsCount();
}

void LoadChar()
{
    saveslot=slot;
    string save="save_";
    if(slot==0)save.append("0");
    if(slot==1)save.append("1");
    if(slot==2)save.append("2");
    save.append(".dat");
    char *save_name = (char*) save.c_str();
    ifstream ifs(save_name);
        ifs >> name;
        ifs >> level;
        ifs >> x;
        ifs >> y;
        ifs >> xp;
        ifs >> nxp;
        ifs >> strength;
        ifs >> vitality;
        ifs >> hp;
        ifs >> maxHp;
        ifs >> attack;
        ifs >> armour;
        ifs >> regeneration;
        ifs >> day[0];
        ifs >> day[1];
        ifs >> damageReduction;
        for (int i=0; i<200; i++) ifs >> enemy.armour[i];
        for (int i=0; i<200; i++) ifs >> enemy.attack[i];
        for (int i=0; i<200; i++) ifs >> enemy.damageReduction[i];
        for (int i=0; i<200; i++) ifs >> enemy.hp[i];
        for (int i=0; i<200; i++) ifs >> enemy.level[i];
        for (int i=0; i<200; i++) ifs >> enemy.maxHp[i];
        for (int i=0; i<200; i++) ifs >> enemy.stats[i];
        for (int i=0; i<200; i++) ifs >> enemy.strenght[i];
        for (int i=0; i<200; i++) ifs >> enemy.vitality[i];
        for (int i=0; i<200; i++) ifs >> enemy.x[i];
        for (int i=0; i<200; i++) ifs >> enemy.y[i];
        for (int i=0; i<200; i++) ifs >> enemy.xpGain[i];
        for(int i=0;i<101;i++)
         for(int j=0;j<101; j++)
            ifs >> enemy.mask[i][j];
        for(int i=0;i<101;i++)
            for(int j=0;j<101; j++)
                ifs >> area[i][j];
    slot=0;
}


void SaveGame()
{
    string save="save_";
    if(slot==0)save.append("0");
    if(slot==1)save.append("1");
    if(slot==2)save.append("2");
    save.append(".dat");
    char *save_name = (char*) save.c_str();
    ofstream ofs(save_name);
    ofs << name << " ";
    ofs << level << " ";
    ofs << x << " ";
    ofs << y << " ";
    ofs << xp << " ";
    ofs << nxp << " ";
    ofs << strength << " ";
    ofs << vitality << " ";
    ofs << hp << " ";
    ofs << maxHp << " ";
    ofs << attack << " ";
    ofs << armour << " ";
    ofs << regeneration << " ";
    ofs << day[0] << " ";
    ofs << day[1] << " ";
    ofs << damageReduction << endl;
    for (int i=0; i<199; i++) ofs << enemy.armour[i] << " ";
    ofs << enemy.armour[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.attack[i] << " ";
    ofs << enemy.attack[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.damageReduction[i] << " ";
    ofs << enemy.damageReduction[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.hp[i] << " ";
    ofs << enemy.hp[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.level[i] << " ";
    ofs << enemy.level[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.maxHp[i] << " ";
    ofs << enemy.maxHp[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.stats[i] << " ";
    ofs << enemy.stats[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.strenght[i] << " ";
    ofs << enemy.strenght[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.vitality[i] << " ";
    ofs << enemy.vitality[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.x[i] << " ";
    ofs << enemy.x[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.y[i] << " ";
    ofs << enemy.y[199] << endl;
    for (int i=0; i<199; i++) ofs << enemy.xpGain[i] << " ";
    ofs << enemy.xpGain[199] << endl;
    for(int i=0;i<101;i++)
     for(int j=0;j<101; j++)
     {
        ofs << enemy.mask[i][j];
        if (j!=100) ofs << " ";
        else ofs << endl;
     }
    for(int i=0;i<101;i++)
     for(int j=0;j<101; j++)
     {
        ofs<<area[i][j];
        if(j==100)ofs<<endl;
     }
}


void LoadMenu()
{
    if(ifstream("save_0.dat").good()==1)
    {
        ifstream ifs0 ("save_0.dat");
        ifs0 >> char0.name;
        ifs0 >> char0.level;
    }
    else strcpy(char0.name,"Пусто");
    if(ifstream("save_1.dat").good()==1)
    {
        ifstream ifs1 ("save_1.dat");
        ifs1 >> char1.name;
        ifs1 >> char1.level;
    }
    else strcpy(char1.name,"Пусто");
    if(ifstream("save_2.dat").good()==1)
    {
        ifstream ifs2 ("save_2.dat");
        ifs2 >> char2.name;
        ifs2 >> char2.level;
    }
    else strcpy(char2.name,"Пусто");
    int done=0;
    slot=0;
    while(done==0)
    {
        wprintf(L"\033[2J\033[1;0H");
        wcout<<L"Выберете слот для загрузки"<<endl;
        if (slot==0) YELLOW;
        wcout<<L"1 "<<char0.name<<L": "<<char0.level<<L" уровень"<<endl;
        if (slot==0) RESET;
        if (slot==1) YELLOW;
        wcout<<L"2 "<<char1.name<<L": "<<char1.level<<L" уровень"<<endl;
        if (slot==1) RESET;
        if (slot==2) YELLOW;
        wcout<<L"3 "<<char2.name<<L": "<<char2.level<<L" уровень"<<endl;
        if (slot==2) RESET;
        switch(getch())
        {
                case 's':
                    if(slot==2)slot=0;
                    else slot++;
                break;
                case '\n':
                    done=1;
                    LoadChar();
                break;
                case 'w':
                    if(slot==0) slot=2;
                    else slot--;
                break;
        }
    }
}




void LevelUp()
{
    level+=1;
    nxp=(nxp*120)/100;
    stats+=3;
    maxHp += 5;
    hp=maxHp;
}

void Hello()
{
    int done=0;
    while(done==0)
    {
        wprintf(L"\033[2J\033[1;0H");
        BOLDYELLOW
        wcout << L"                             Добро пожаловать в" << endl;
        wcout << L"      @@    @@                          @@                              @@" << endl;
        wcout << L"      @@    @@                          @@                              @@" << endl;
        wcout << L"      @@    @@  @@@@@@  @@@@@@@  @@@@@  @@       @@@@@     @@@@     @@@@@@" << endl;
        wcout << L"      @@@@@@@@ @@@  @@@ @@@  @@ @@   @@ @@      @@   @@    @@  @@ @@    @@" << endl;
        wcout << L"      @@    @@ @@@@@@   @@@     @@   @@ @@      @@   @@    @@  @@ @@    @@" << endl;
        wcout << L"      @@    @@ @@@      @@@     @@   @@ @@   @@ @@   @@  @ @@  @@ @@    @@" << endl;
        wcout << L"      @@    @@  @@@@@@  @@@      @@@@@  @@@@@@@  @@@@@ @@  @@  @@  @@@@@@@" << endl << endl;
        RESET
        wcout << L"Выбор по WASD, сохранение и загрузка t/p, статы M" << endl << endl;
        if(slot==0)YELLOW
        wcout << L"Новая игра" << endl;
        if(slot==0)RESET
        if(slot==1)YELLOW
        wcout << L"Загрузка" << endl;
        if(slot==1)RESET
        if(slot==2)YELLOW
        wcout << L"Дебаг" << endl;
        if(slot==2)RESET

        switch(getch())
        {
            case 's':
                if(slot!=2)slot++;
                else slot=0;
            break;
            case '\n':
                done=1;
            break;
            case 'w':
                if(slot==0)	slot=2;
                else slot--;
            break;
        }
    }
}

int main()
{
    setlocale(LC_CTYPE,"");
    wcout<<L"Разверните терминал и не изменяйте его размеры после этого, это необходимо для корректного" <<
           L"отображения игры. Нажмите любую клавишу для продолжения...";
    getch();
    ioctl(0, TIOCGWINSZ, &w);
    height=w.ws_row; //40 24
    width=w.ws_col; //168 80
    wcout << height<< L" " << width;
    getch();
    if(height<20 || width<70)
    {
        wcout<<endl<<L"слишком мало места :("<<endl;
        return 1;
    }
    srand(time(NULL));
    Hello();
    if(slot==0)
    {
        NewGame();
        x=Randomizer(100);
        y=Randomizer(100);
        MapCreate();
        SaveGame();
        saveslot=slot;
    }
    if(slot==1)LoadMenu();
    if(slot==2)       //Дебаг
    {

    }
    while(hp>0)
    {
        usedheight=0; usedwidth=0;
        DrawTopBar();
        MapShow();
        DrawBotBar();
        if(nothingIsDone==false)
        {
            day[1]++;
            if(day[1]>=10)
            {
                day[0]++;
                day[1]=0;
            }
            if (day[0]==prev+1)
            {
                prev++;
                EnemyCreate();
            }
            if (hp<maxHp) hp+=regeneration;
            if (hp>maxHp) hp=maxHp;
            if (enemy.mask[y][x]!=0)
            {
                Battle();
                SidePrint(L"\033[2;0HНа вас напали!");
            }
        }
        switch(getch())
        {
            case 'w':
                if(y<100)
                {
                    y++;
                    nothingIsDone=false;
                }
                else nothingIsDone=true;
            break;
            case 'a':
                if(x>0)
                {
                    x--;
                    nothingIsDone=false;
                }
                else nothingIsDone=true;
            break;
            case 's':
                if(y>0)
                {
                    y--;
                    nothingIsDone=false;
                }
                else nothingIsDone=true;
            break;
            case 'd':
                if(x<100)
                {
                    x++;
                    nothingIsDone=false;
                }
                else nothingIsDone=true;
            break;
            case 'm':
                nothingIsDone=true;
                done=0;
                wprintf(L"\033[1;0H");
                for(int i=0;i<height-2;i++)wcout<<endl;
                while(done!=1)
                {
                    wcout<<L"\r";
                    if(slot==0)YELLOW; wcout<<L"Сохранить   ";	if(slot==0)RESET;
                    if(slot==1)YELLOW; wcout<<L"Загрузка   "; 	if(slot==1)RESET;
                    if(slot==2)YELLOW; wcout<<L"Отмена";          if(slot==2)RESET;
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
                if (slot==1)LoadMenu();
            break;
            default:
                nothingIsDone=true;
            break;
        }
    }
    if (hp<=0) wcout << L"Вы погибли. =(";
    return 0;
}
