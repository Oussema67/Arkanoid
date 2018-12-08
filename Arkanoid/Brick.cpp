#include "Brick.h"


Brick::Brick(){
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	alive = true;

}

bool Brick::BrickCollision(int ball_x, int ball_y, int ball_w,int ball_h){
	if (ball_y + ball_h < this->y || ball_y > this->y + this->h || ball_x + ball_w < this->x || ball_x > this->x + this->w)
		return false;
	else
		return true;
}


