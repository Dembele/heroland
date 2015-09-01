static struct termios old, nov;

void initTermios(int echo) 
{
  tcgetattr(0, &old);
  nov = old;
  nov.c_lflag &= ~ICANON;
  nov.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &nov);
}

void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char getch(void) 
{
  return getch_(0);
}

char getche(void) 
{
  return getch_(1);
}
