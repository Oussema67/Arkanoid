#ifndef BRICK_H
#define BRICK_H

#include <SDL.h>
#include <iostream>


class Brick 
{
	public:
		
		Brick();
		bool alive;
		int color;
		int x;
		int y;
		int w;
		int h;
		bool BrickCollision(int ball_x, int ball_y, int ball_w,int ball_h);
};

#endif	
