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
#include "defines.h"
#include "getch.h"

using namespace std;

struct winsize w;

struct character
{
int stats, hp, maxHp, strength, vitality, level, x, y, xp, nxp, attack, regeneration, armour;
double damageReduction;
char name[100], area[101][101];
} char0, char1, char2;

struct enemy
{
int stats[200], hp[200], maxHp[200], strenght[200], vitality[200], level[200], x[200], y[200], xpGain[200], attack[200], armour[200];
double damageReduction[200];
int mask[101][101];
} enemy;

int id, stats, hp, maxHp, strength, vitality, level, x, y, xp, nxp, attack, armour, regeneration;
double damageReduction;
char name[100], area[101][101];

int slot, temp, done, height, width, usedwidth, usedheight, saveslot, day;

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

//todo: x>86 map adds one collumn;
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
                    if (enemy.mask[i][j]>=level+7) RED;
                    if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                    if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                    cout << area[i][j];
                    if (enemy.mask[i][j]!=0) RESET;
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
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            cout << endl;
        }
    }
    if (x >= 86 && y >= 93){											//верх справа угол
        for (int i = 100; i != 86; i--)
        {
            cout.width(width-30);
            cout<<right;
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
            for (int j = 71; j <= 100; j++)
            {
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
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
                if (enemy.mask[i][j]>=level+7) RED;
                if (enemy.mask[i][j]<=level-7 && enemy.mask[i][j]!=0) GREEN;
                if (enemy.mask[i][j]>level-7 && enemy.mask[i][j]<level+7 && enemy.mask[i][j]!=0) YELLOW;
                cout << area[i][j];
                if (enemy.mask[i][j]!=0) RESET;
            }
            cout << endl;
        }
    }
    area[y][x] = temp;
}

void SidePrint(wstring st)
{
    printf("\033[2;0H");
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
        if(usedheight==height-3)
        {
            cout<<"Место на экране закончилось... нажмите любую клавишу для продолжения";
            getch();
            printf("\033[2;0H");
            usedheight=0;
            usedwidth=0;
        }
    }
}

void DrawBotBar()
{
    string placebottom = "\033[";
    placebottom.append(ToStr(height));
    placebottom.append(";0H");
    cout<<placebottom;
    cout<<"Уровень: "<<level<<" Опыт: "<<xp<<"/"<<nxp<<" Здоровье: "<<hp;
    cout<<"/"<<maxHp << " Атака: "<<attack<<" Броня: "<<damageReduction;
}

void MapCreate()
{
    for(int i=0; i<=100; i++)
        for(int j=0; j<=100; j++)
        {
            int compare=randomizer(100);
            if (i==0 && j==0) area[i][j]=randomizer(7);
            if (i==0 && j>0 && compare>75) area[i][j]=randomizer(7);
            if (i==0 && j>0 && compare<=75) area[i][j]=area[i][j-1];
            if (i>0 && j==0 && compare>75) area[i][j]=randomizer(7);
            if (i>0 && j==0 && compare<=75) area[i][j]=area[i-1][j];
            if (i>0 && j>0 && area[i-1][j]==area[i][j-1]) area[i][j]=area[i-1][j];
            if (i>0 && j>0 && area[i-1][j]!=area[i][j-1])
            {
                if (compare<=40) area[i][j]=area[i-1][j];
                if (compare>40 && compare<80) area[i][j]=area[i][j-1];
                if (compare>=80) area[i][j]=randomizer(7);
            }
        }
        for(int i=0; i<=100; i++)
            for(int j=0; j<=100; j++)
                if (randomizer(100)>90) area[i][j]=8;

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
    printf("\033[2J\033[1;0H");
    cout << "Выберите слот для сохранения:" << endl;
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
        printf("\033[2J\033[1;0H");
        if (slot==0)YELLOW;
        cout<<char0.name<<" "<<char0.level<<" уровень"<<endl;
        if (slot==0)RESET;
        if (slot==1)YELLOW;
        cout<<char1.name<<" "<<char1.level<<" уровень"<<endl;
        if (slot==1)RESET;
        if (slot==2)YELLOW;
        cout<<char2.name<<" "<<char2.level<<" уровень"<<endl;
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
    cout<<endl;
    cout<<"Введите имя персонажа: ";
    cin>>name;
    stats=15;
    int old_slot=slot;
    while (stats>=1)
    {
        printf("\033[2J\033[1;0H");
        if (slot==0) YELLOW;
        cout<<"Сила        : ";
        for(int i=0; i<strength; i++) cout<< "*";
        cout<< endl;
        if (slot==0) RESET;
        if (slot==1) YELLOW;
        cout<<"Выносливость: ";
        for(int i=0; i<vitality; i++) cout<< "*";
        cout<< endl;
        if (slot==1) RESET;
        cout << "Cвободные очки: ";
        for(int i=0; i<stats; i++) cout<< "*";
        cout<< endl;
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
    printf("\033[2J\033[1;0H");
        cout<<name<< ", на старте у вас будет Сила: " << strength << " Выносливость: " << vitality << endl;
        level               = 1;
        nxp                 = 100;
        armour              = 16;
        attack              = strength * 2;
        damageReduction     = (armour/(armour+100));
}

void Load_char()
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
    ofs << damageReduction << " ";
    ofs << endl;
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


void Load_menu()
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
        printf("\033[2J\033[1;0H");
        cout<<"Выберете слот для загрузки"<<endl;
        if (slot==0) YELLOW;
        cout<<"1 "<<char0.name<<": "<<char0.level<<" уровень"<<endl;
        if (slot==0) RESET;
        if (slot==1) YELLOW;
        cout<<"2 "<<char1.name<<": "<<char1.level<<" уровень"<<endl;
        if (slot==1) RESET;
        if (slot==2) YELLOW;
        cout<<"3 "<<char2.name<<": "<<char2.level<<" уровень"<<endl;
        if (slot==2) RESET;
        switch(getch())
        {
                case 's':
                    if(slot==2)slot=0;
                    else slot++;
                break;
                case '\n':
                    done=1;
                    Load_char();
                break;
                case 'w':
                    if(slot==0) slot=2;
                    else slot--;
                break;
        }
    }
}




void levelup()
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
        printf("\033[2J\033[1;0H");
        BOLDYELLOW
        cout << "                             Добро пожаловать в" << endl;
        cout << "      @@    @@                          @@                              @@" << endl;
        cout << "      @@    @@                          @@                              @@" << endl;
        cout << "      @@    @@  @@@@@@  @@@@@@@  @@@@@  @@       @@@@@     @@@@     @@@@@@" << endl;
        cout << "      @@@@@@@@ @@@  @@@ @@@  @@ @@   @@ @@      @@   @@    @@  @@ @@    @@" << endl;
        cout << "      @@    @@ @@@@@@   @@@     @@   @@ @@      @@   @@    @@  @@ @@    @@" << endl;
        cout << "      @@    @@ @@@      @@@     @@   @@ @@   @@ @@   @@  @ @@  @@ @@    @@" << endl;
        cout << "      @@    @@  @@@@@@  @@@      @@@@@  @@@@@@@  @@@@@ @@  @@  @@  @@@@@@@" << endl << endl;
        RESET
        cout << "Выбор по WASD, сохранение и загрузка t/p, статы M" << endl << endl;
        if(slot==0)YELLOW
        cout << "Новая игра" << endl;
        if(slot==0)RESET
        if(slot==1)YELLOW
        cout << "Загрузка" << endl;
        if(slot==1)RESET
        switch(getch())
        {
            case 's':
                if(slot==0)slot++;
                else slot=0;
            break;
            case '\n':
                done=1;
            break;
            case 'w':
                if(slot==1)	slot--;
                else slot=1;
            break;
        }
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
        maxHp=vitality*2+1;
        hp=maxHp;
        MapCreate();
        SaveGame();
        saveslot=slot;
    }
    if(slot==1)Load_menu();
    while(hp>0)
    {
        day++;
        printf("\033[2J\033[1;0H");
        cout<<"x: "<<x<<" y: "<<y;
        switch(area[y][x])
        {
            case '@':
                cout<<" Вы входите в лес"<<endl;
            break;
            case '.':
                cout<<" Вы входите в пустыню"<<endl;
            break;
            case '*':
                cout<<" Вы входите в джунгли"<<endl;
            break;
            case '^':
                cout<<" Вы поднимаетесь в горы"<<endl;
            break;
            case '#':
                cout<<" Вы поднимаетесь на плато"<<endl;
            break;
            case '!':
                cout<<" Вы входите в болото"<<endl;
            break;
            case '%':
                cout<<" Вы поднимаетесь в холмы"<<endl;
            break;
            case '~':
                cout<<" Вы выходите к водоему"<<endl;
            break;
        }
        Map_Show();
        if(day%10==0)EnemyCreate();
        if (hp<maxHp) hp+=regeneration;
        if (hp>maxHp) hp=maxHp;
        DrawBotBar();
        switch(getch())
        {
            case 'w':
                if(y<100)y++;
            break;
            case 'a':
                if(x>0)x--;
            break;
            case 's':
                if(y>0)y--;
            break;
            case 'd':
                if(x<100)x++;
            break;
            case 'm':
                done=0;
                printf("\033[1;0H");
                for(int i=0;i<height-2;i++)cout<<endl;
                while(done!=1)
                {
                    cout<<"\r";
                    if(slot==0)YELLOW; cout<<"Сохранить   ";	if(slot==0)RESET;
                    if(slot==1)YELLOW; cout<<"Загрузка   "; 	if(slot==1)RESET;
                    if(slot==2)YELLOW; cout<<"Отмена";          if(slot==2)RESET;
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
