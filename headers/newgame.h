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
		if (slot==0)YELLOW
		cout<<char0.name<<" "<<char0.level<<" уровень"<<endl;
		if (slot==0)RESET
		if (slot==1)YELLOW
		cout<<char1.name<<" "<<char1.level<<" уровень"<<endl;
		if (slot==1)RESET
		if (slot==2)YELLOW
		cout<<char2.name<<" "<<char2.level<<" уровень"<<endl;
		if (slot==2)RESET
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
		if (slot==0) YELLOW
		cout<<"Сила        : ";
		for(int i=0; i<strength; i++) cout<< "*";
		cout<< endl;
		if (slot==0) RESET
		if (slot==1) YELLOW
		cout<<"Выносливость: ";
		for(int i=0; i<vitality; i++) cout<< "*";
		cout<< endl;
		if (slot==1) RESET
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
		level=1;
		nxp=100;
		armour = 16;
		armreduction = armour;
}
