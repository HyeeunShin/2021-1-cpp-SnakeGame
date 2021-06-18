#include "game_board.h"

// 생성자
//
Game_board::Game_board() : init_x{3}, init_y{3}, max_x{70}, max_y{30}, point{0} {}
// get_init_x()
//
const int Game_board::get_init_x() const
{
    return init_x;
}

// get_init_y()
//
const int Game_board::get_init_y() const
{
    return init_y;
}

// get_max_x()
//
const int Game_board::get_max_x() const
{
    return max_x;
}

// get_max_y()
//
const int Game_board::get_max_y() const
{
    return max_y;
}

// set_point()
//
void Game_board::set_point(int num){
	point += num;
}

// get_point()
//
int Game_board::get_point(){
	return point;
}

// 게임 prompt 화면 출력
//
void Game_board::print_prompt(){
	resize_term(40, 100);
	border('*','*','*','*','*','*','*','*');
	mvprintw(1,10, "<Snake Game>");
	refresh();
	getch();
}

// 벽 생성 함수
//
void Game_board::print_wall(){
	w = (char)219;	// Wall
	im_w = 'M'; 	// Immune wall

	// 게임 실행되는 윈도우
	win1 = newwin(max_y, max_x, init_y, init_x);
	
	// Wall
        // up-horizontal
        for(int i = init_x + 1; i < max_x; i++)
        {
		move(init_y,i);
		addch(w);
        }
        //left-vertical
        for(int i = init_y + 1; i < max_y; i++)
        {
		move(i, init_x);
		addch(w);
        }
        //down-horizontal
        for(int i = init_x + 1; i < max_x; i++)
        {
		move(max_y,i);
		addch(w);
        }
        //right-vertical
        for(int i = init_y + 1; i < max_y; i++)
        {
		move(i, max_x);
		addch(w);
        }

	// Immune Wall
	// left top
	move(init_y, init_x);
	addch(im_w);
	// left bottom
	move(max_y, init_x);
	addch(im_w);
	// right top
	move(init_y, max_x);
	addch(im_w);
	// right bottom
	move(max_y, max_x);
	addch(im_w);

	wrefresh(win1);
}

// 점수판 생성 함수
//
void Game_board::print_score(){
	start_color();
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	
	// 점수판 표시하는 윈도우
	win2 = newwin(20, 25, 3, 73);
	wbkgd(win2, COLOR_PAIR(2));
	wattron(win2, COLOR_PAIR(2));
	wborder(win2, '|', '|', '-', '-', '*', '*', '*', '*');

	// 점수 표시
	mvprintw(5, 75, "score : %d", point);

	wrefresh(win2);
}

