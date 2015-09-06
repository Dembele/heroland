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

