#include "snake.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
snakepart::snakepart(int col,int row)
{
	x=col;
	y=row;
}

snakepart::snakepart()
{
	x=0;
	y=0;
}
snakeclass::snakeclass()
{
	initscr();
	nodelay(stdscr,true);           //if there wasn't any key pressed don't wait for keypress
	keypad(stdscr,true);            //init the keyboard
	noecho();                                   //don't write
	curs_set(0);                            //cursor invisible
	
	setlocale(LC_ALL, "");	// 유니코드 사용 위한 함수
	
	start_color();	// 색 쓰기 위한 함수


	//getmaxyx(stdscr,maxheight,maxwidth);	// set the window size
	initheight = game_board.get_init_y();
	initwidth = game_board.get_init_x();
	maxheight = game_board.get_max_y();
	maxwidth = game_board.get_max_x();

	// set the character of snake, food, poison
	partchar='x'; //snake
	etel='*'; //food
	pstel='@'; //poison

	//initialize food location
	food.x=0;
	food.y=0;

	//initialize poison location
	poison.x=0;
	poison.y=0;

	//처음 뱀은 5칸짜리로 시작
	for(int i=0;i<5;i++)
		snake.push_back(snakepart(40+i,10));

	// point 초기화
	del=110000;
	get=0; //get boolean initialation
	lost = 0; //lost boolean initialation

	// 처음 방향은 왼쪽으로 초기화
	direction='l';
	srand(time(NULL));
	putfood();
	putpoison();
	
	// Game prompt
	game_board.print_prompt();
	
	// Make the game-board
	game_board.print_wall();
	
	//draw the snake
	for(int i=0;i<snake.size();i++)
	{
	move(snake[i].y,snake[i].x);
	addch(partchar);
	}

	move(food.y,food.x);
	move(poison.y, poison.x);
	addch(etel);

	//addch(pstel);
	refresh();
}

snakeclass::~snakeclass()
{
    nodelay(stdscr,false);          //turn back
    getch();                                        //wait until a key is pressed
    endwin();
}

//food 생성 함수
void snakeclass::putfood()
{
    while(1)
    {
        int tmpx = rand() % maxwidth + initwidth; // initwidth ~ width
        int tmpy = rand() % maxheight + initheight; // initheight ~ height

        for(int i=0;i<snake.size();i++)
            if(snake[i].x==tmpx && snake[i].y==tmpy)
                continue;
        if(tmpx>=maxwidth || tmpy>=maxheight || tmpx<=initwidth || tmpy<=initheight)
            continue;
        food.x=tmpx;
        food.y=tmpy;
        break;
    }
    move(food.y,food.x);
    addch(etel);
    refresh();
}

//poison 생성 함수
void snakeclass::putpoison()
{
  while(1)
  {
      int tmpx=rand()%maxwidth+initwidth;
      int tmpy=rand()%maxheight+initheight;
      for(int i=0;i<snake.size();i++)
          if(snake[i].x==tmpx && snake[i].y==tmpy)
              continue;
      if(tmpx>=maxwidth || tmpy>=maxheight || tmpx<=initwidth || tmpy<=initheight)
          continue;
      poison.x=tmpx;
      poison.y=tmpy;
      break;
  }
  move(poison.y,poison.x);
  addch(pstel);
  refresh();
}

//food, poison, wall과 충돌할 경우
bool snakeclass::collision()
{
	//snake가 벽과 충돌할 경우	
	if(snake[0].x==initwidth || snake[0].x==maxwidth || snake[0].y==initheight || snake[0].y==maxheight)
		return true;

	//snake가 자기 자신과 충돌할 경우
	for(int i=2;i<snake.size();i++)
	{
		if(snake[0].x==snake[i].x && snake[0].y==snake[i].y)
			return true;
	}

    //collision with the food
	if(snake[0].x==food.x && snake[0].y==food.y)
	{
		get = true;
		putfood();
		
		game_board.set_point(10);
		move(5, 84);
		printw("%d", game_board.get_point());
	
		if((game_board.get_point() % 100)==0)
			del-=10000;	// 점수 높아질 수록 속도 빨라진다

		return false;

	}
	
	//collision with the poison
	else if(snake[0].x==poison.x && snake[0].y==poison.y)
	{
		get = false;
		putpoison();

		game_board.set_point(-10);
		move(5, 84);
		printw("%d", game_board.get_point());
	
		if((game_board.get_point() % 100)==0)
			del+=10000;	// 점수 낮아질 수록 속도 느려진다

		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		printw(" ");
		snake.pop_back();
	}
	
	else{
		get = false;
	}

	return false;
}


void snakeclass::movesnake()
{
    //detect key
    int tmp=getch();
    switch(tmp)
    {
        case KEY_LEFT:
            if(direction!='r')
                direction='l';
            break;
        case KEY_UP:
            if(direction!='d')
                direction='u';
            break;
        case KEY_DOWN:
            if(direction!='u')
                direction='d';
            break;
        case KEY_RIGHT:
            if(direction!='l')
                direction='r';
            break;
        case KEY_BACKSPACE:
            direction='q';
            break;
    }
    //if there wasn't a collision with food
    //food를 먹었을 때는 밑의 if문이 실행하지 않으며 뒤에 빼는 작업 X
	if(!get)
	{
		move(snake[snake.size()-1].y,snake[snake.size()-1].x);
		printw(" ");
		snake.pop_back();
	}

    //direction func
    if(direction=='l')
    {
        snake.insert(snake.begin(),snakepart(snake[0].x-1,snake[0].y));
    }else if(direction=='r'){
        snake.insert(snake.begin(),snakepart(snake[0].x+1,snake[0].y));

    }else if(direction=='u'){
        snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y-1));
    }else if(direction=='d'){
        snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y+1));
    }

	move(snake[0].y,snake[0].x);
	addch(partchar);

    refresh();
}

void snakeclass::start()
{
    while(1)
    {
	// 벽 또는 자기 자신과 충돌 발생하면 게임 종료
        if(collision())
        {
            move(12,36);
            printw("game_over");
            break;
        }

        movesnake();	//스네이크 이동

        if(direction=='q')              //exit
            break;
        usleep(del);            //Linux delay
    }
}
