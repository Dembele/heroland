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
		ifs >> armreduction;
		for(int i=0;i<101;i++)
			for(int j=0;j<101; j++)
				ifs >> area[i][j];
	slot=0;
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
		if (slot==0) YELLOW
		cout<<"1 "<<char0.name<<": "<<char0.level<<" уровень"<<endl;
		if (slot==0) RESET
		if (slot==1) YELLOW
		cout<<"2 "<<char1.name<<": "<<char1.level<<" уровень"<<endl;
		if (slot==1) RESET
		if (slot==2) YELLOW
		cout<<"3 "<<char2.name<<": "<<char2.level<<" уровень"<<endl;
		if (slot==2) RESET
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

void SaveGame()
{
	string save="save_";
	if(slot==0)save.append("0");
	if(slot==1)save.append("1");
	if(slot==2)save.append("2");
	save.append(".dat");
	char *save_name = (char*) save.c_str();
	ofstream ofs(save_name);
	ofs << name << endl;
	ofs << level << endl;
	ofs << x << endl;
	ofs << y << endl;
	ofs << xp << endl;
	ofs << nxp << endl;
	ofs << strength << endl;
	ofs << vitality << endl;
	ofs << hp << endl;
    ofs << maxHp << endl;
	ofs << attack << endl;
	ofs << armour << endl;
	ofs << regeneration << endl;
	ofs << armreduction << endl;
	for(int i=0;i<101;i++)
	 for(int j=0;j<101; j++)
	 {
		ofs<<area[i][j];
		if(j==100)ofs<<endl;
	 }
}

