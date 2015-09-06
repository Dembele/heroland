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

