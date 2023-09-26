#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

using namespace std;

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

enum CellType { Empty, Figure, Bottom };

void draw(CellType **field, int n, int m) {
  for (int i = 0;i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      switch (field[i][j]) {
	case Empty:
	  cout << " ";
	  break;
	case Figure:
	  cout << "*";
	  break;
	case Bottom:
	  cout << "#";
	  break;
      }
    }
    cout << endl;
  }
}

bool moveRight(CellType **a, int n, int m) {
  for (int j = m - 1; j >= 0; --j) {
    for (int i = 0; i < n; ++i) {
      if (a[i][j] == Figure) {
	if (j == (m - 1)) return false; // плохой код, ты ведь пишешь isFigureCanMove
	a[i][j] = Empty;
	a[i][j + 1] = Figure;
      }
    }
  }
  return true;
}
  

int main() {

  CellType **field;
  
  const int n = 10, m = 20;
  
  field = new CellType* [n];
  for (int i = 0;i < n; ++i) {
    field[i] = new CellType[m];
    for (int j = 0; j < m; ++j) {
      field[i][j] = Empty;
    }
  }
  
  // как-то добавляешь фигуру
  field[2][2] = Figure;
  field[2][3] = Figure;
  field[3][2] = Figure;
  
  bool canMove = true;
  do {
    system("clear");
    draw(field, n, m);
    canMove = moveRight(field, n, m);
    system("sleep 1");
  } while (canMove);
  
      
}