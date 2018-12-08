#include <SDL.h>
#include "Brick.h"
#include <iostream>
#include <vector>

int x_vault;

std::vector<Brick*> blocks;
SDL_Rect ball;
struct { int x; int y; } speed;
int score = 0;
SDL_Window* pWindow = nullptr;
SDL_Surface* win_surf = nullptr;
SDL_Surface* plancheSprites = nullptr;
SDL_Surface * Sprites_vaiss = nullptr;

SDL_Rect srcBg = { 0,128, 96,128 }; // x,y, w,h (0,0) en haut a gauche
SDL_Rect srcBall = { 80,66,16,13 };
SDL_Rect scrVaiss = { 384,143,74,16 };
SDL_Rect Block = { 0,0,30,16 };

void init()
{
	pWindow = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);

	plancheSprites = SDL_LoadBMP("./sprites.bmp");
	SDL_SetColorKey(plancheSprites, true, 0);  // 0: 00/00/00 noir -> transparent
	Sprites_vaiss = SDL_LoadBMP("./Arkanoid_sprites.bmp");
	SDL_SetColorKey(Sprites_vaiss, true, 0);
	ball.x = win_surf->w / 2;
	ball.y = win_surf->h / 2 + 50;
	speed.x =  5;
	speed.y = 7;
}


// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
	// remplit le fond 
	SDL_Rect dest = { 0,0,0,0 };
	for (int j = 0; j < win_surf->h; j+=128)
		for (int i = 0; i < win_surf->w; i += 96)
		{
			dest.x = i;
			dest.y = j;
			SDL_BlitSurface(plancheSprites, &srcBg, win_surf, &dest);
		}
	// affiche balle
	SDL_BlitSurface(Sprites_vaiss, &srcBall, win_surf, &ball);

	dest = { 0,0,30,16 };
	for(int i=0; i< blocks.size();i++)
	{
			if(blocks[i]->alive == true)
			{
				dest.x=blocks[i]->x;
				dest.y=blocks[i]->y;
				SDL_BlitSurface(Sprites_vaiss, &Block, win_surf, &dest);
			}

	}
	// dedplacement
	ball.x += speed.x;
	for(int i=0;i< blocks.size();i++)
	{
		if(blocks[i]->alive == true)
		{
			if (blocks[i]->Brick::BrickCollision(ball.x, ball.y, ball.w/2, ball.h/2) == true)
			{
				speed.x = -speed.x;
				blocks[i]->alive = false;
				break;
			}
		}
	}
	ball.y += speed.y;
	for(int i=0;i< blocks.size();i++)
	{
		if(blocks[i]->alive == true)
		{
			if (blocks[i]->Brick::BrickCollision(ball.x,ball.y,ball.w/2,ball.h/2) == true)
			{
				speed.y = -speed.y;
				blocks[i]->alive = false;
				score += 100;
				std::cout << score << "\n";
				break;
			}
		}
	}

	// collision bord
	if ((ball.x < 1) || (ball.x > (win_surf->w - 25)))
		speed.x *= -1;
	if ((ball.y < 1) || (ball.y > (win_surf->h - 25)))
		speed.y *= -1;

	// touche bas -> rouge
	/*if (ball.y >(win_surf->h - 25))
		srcBall.y = 64;
	*/

	// collision vaisseau
	if ((ball.x > x_vault) && (ball.x < x_vault+128) && (ball.y > win_surf->h - 32 -20))
	{
		speed.y *= -1;
		//srcBall.y = 96;
		scrVaiss = { 384,239,128,17 };
		 // -> vert
	}
	
	// vaisseau
	dest.x = x_vault;
	dest.y = win_surf->h - 32;
	SDL_BlitSurface(Sprites_vaiss, &scrVaiss, win_surf, &dest);
}




int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
		std::cerr <<"Echec de l'initialisation de la SDL "<<SDL_GetError() << std::endl;
		return 1;
    }

	init();
	SDL_Rect dest = { 0,0,30,16 };
	for (int i = 1; dest.y < win_surf->h/2; i++,dest.x+=30)
		{
			if (dest.x > win_surf->w)
			{
				dest.x = 0;
				dest.y += 16;
			}
			Brick * b = new Brick();
			b->x = dest.x;
			b->y = dest.y;
			b->w = 30;
			b->h = 16;
			b->alive = true;
			blocks.push_back(b);
			SDL_BlitSurface(Sprites_vaiss, &Block, win_surf, &dest);
		}
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					// touche clavier
					case SDLK_LEFT:  x_vault -= 10; break;
					case SDLK_RIGHT: x_vault +=10; break;
					case SDLK_ESCAPE: quit = true; break;
					default: break;
				}
				break;
			case SDL_MOUSEMOTION:	x_vault += event.motion.xrel;	break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "mouse click " << event.button.button << std::endl;
				break;
			default: break;
			}
		}
		draw();
		SDL_UpdateWindowSurface(pWindow); 
		SDL_Delay(20); // 50 fps
	}
    SDL_Quit();
    return 0;
}
