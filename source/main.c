#include "gba_macros.h"

//define basic types
#include <stdint.h>
#include <stdbool.h>

#pragma region Tutorial_000_code

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef int8_t		s8;
typedef int16_t		s16;
typedef	int32_t		s32;
//define volatiles
typedef volatile uint8_t		vu8;
typedef volatile uint16_t		vu16;
typedef volatile uint32_t		vu32;
typedef volatile int8_t			vs8;
typedef volatile int16_t		vs16;
typedef	volatile int32_t		vs32;


#define REG_DISPCTN *((vu32*)(0x04000000))
#define VIDEOMODE_3 0x0403
#define BG_ENABLE2 0x0400

#define SCREENBUFFER ((vu16*)0x06000000)
#define SCREEN_W 240
#define SCREEN_H 160

u16 setColor(u8 a_red, u8 a_green, u8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1F) << 10;
}

void plotPixel( s32 a_x, s32 a_y, u16 a_color)
{
	SCREENBUFFER[a_y * SCREEN_W + a_x] = a_color;
}

void drawRect(s32 a_left, s32 a_top, s32 a_width, s32 a_height, u16 a_color)
{
	for (s32 y = 0; y < a_height; ++y)
	{
		for (s32 x = 0; x < a_width; ++x)
		{
			plotPixel(a_left + x, a_top + y, a_color);
		}
	}	
}

s32 abs(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}

void drawLine(s32 a_x1, s32 a_y1, s32 a_x2, s32 a_y2, u16 a_color)
{
	//get horizontal and vertical displacement of the line
	s32 w = a_x2 - a_x1;
	s32 h = a_y2 - a_y1;

	//work out the change in the x and y with these delta values
	s32 dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

	if (w < 0) dx1 = dx2 -1; else if (w > 0) dx1 = dx2 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	//figure out which is the longest displacement the horizontal or the vertical
	s32 longest = abs(w);
	s32 shortest = abs(h);
	//test to make sure assumption is correct
	if (shortest > longest)
	{
		//use xor to swap values
		longest ^= shortest; shortest ^= longest; longest ^= shortest;

		if (h < 0) dy2 = -1; else if ( h > 0) dy2 = 1;
		dx2 = 0;
	}
	//get half of the longest displacement
	s32 numenerator = longest >> 1;
	for (s32  i = 0; i < longest; ++i)
	{
		plotPixel(a_x1, a_y1, a_color);
		numenerator += shortest;
		if (numenerator > longest)
		{
			numenerator -= longest;
			a_x1 += dx1;
			a_y1 += dy1;
		}
		else
		{
			a_x1 += dx2;
			a_y1 += dy2;
		}
	}
}
#pragma endregion

#pragma region RandomNumberGeneration
s32 __rand_seed = 42;

s32 seed_gba_rand(s32 a_val)
{
	s32 old = __rand_seed;
	__rand_seed = a_val;
	return old;
}

//Generate a random value using LCB values taken fromo the numerical recipies book
s32 gba_rand()
{
	__rand_seed = 1664525 * __rand_seed + 1013904223;
	return (__rand_seed >> 16) & 0x7FFF;
}

//random value within a range
s32 gba_rand_range(s32 a_min, s32 a_max)
{
	return (gba_rand()*(a_max - a_min) >> 15) + a_min;
}
#pragma endregion

#pragma region Pong_Ball

typedef struct Ball
{
	s32 x, y, xDir, yDir, size;
	u16 colour;
} Ball;

void StartBall (Ball* a_ball)
{
	while (a_ball -> xDir == 0)
	{
		a_ball -> xDir = gba_rand_range(-1,2);
	}
	a_ball->yDir = gba_rand_range(-1,2);
}

void InitBall( Ball* a_ball, s32 a_x, s32 a_y, s32 a_size, u16 a_colour)
{
	a_ball->x = a_x;
	a_ball->y = a_y;
	a_ball->size = a_size;
	a_ball->colour = a_colour;
	a_ball->xDir = a_ball->yDir = 0;
	StartBall(a_ball);
}

void MoveBall(Ball* a_ball)
{
	a_ball->y += a_ball->yDir;
	if (a_ball->y <0)
	{
		a_ball->y = 0;
		a_ball->yDir *= -1;
	}
	if (a_ball->y > SCREEN_H - a_ball->size)
	{
		a_ball->y = SCREEN_H - a_ball->size;
		a_ball->yDir *= -1;
	}
	
	a_ball->x += a_ball->xDir;

	if (a_ball->x <0 || a_ball->x > SCREEN_W - a_ball->size)
	{
		a_ball->x = (SCREEN_W >> 1) - (a_ball->size >> 1);
		a_ball->y = (SCREEN_H >> 1) - (a_ball->size >> 1);
		a_ball->xDir = 0; a_ball->yDir = 0;
		StartBall(a_ball);
	}
}

void DrawBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, a_ball->colour);
}

void ClearBall(Ball* a_ball)
{
	drawRect(a_ball->x, a_ball->y, a_ball->size, a_ball->size, setColor(0,0,0));
}

#pragma endregion

#pragma region Pong_Paddle

typedef struct Paddle
{
	s32 x, y, width, height;
	u16 colour;
}Paddle;

void InitPaddle(Paddle* a_paddle, s32 a_x, s32 a_y, s32 a_width, s32 a_height, u16 a_colour)
{
	a_paddle->x = a_x;
	a_paddle->y = a_y;
	a_paddle->width = a_width;
	a_paddle->height = a_height;
	a_paddle->colour = a_colour;
}

void DrawPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, a_paddle->colour);
}

void ClearPaddle(Paddle* a_paddle)
{
	drawRect(a_paddle->x, a_paddle->y, a_paddle->width, a_paddle->height, setColor(0,0,0));
}

void MovePaddle(Paddle* a_paddle)
{

}

#pragma endregion

#define REG_VCOUNT (*(vu16*)(0x04000006))
void vsync()
{
	while (REG_VCOUNT >= SCREEN_H);
	while (REG_VCOUNT < SCREEN_H);	
}

int main()
{
	//set GBA rendering context to MODE 3 Bitmap Rendering
	REG_DISPCTN = VIDEOMODE_3 | BG_ENABLE2;

	seed_gba_rand(23343);
	Ball ball;
	InitBall(&ball, SCREEN_W >> 1, SCREEN_H >> 1, 10, setColor(31,31,31));

	Paddle p1;
	InitPaddle(&p1, 10,60,8,40, setColor(0,0,31));
	Paddle p2;
	InitPaddle(&p2, SCREEN_W - 18,60,8,40, setColor(31,0,0));
	// s32 t = 0;
	while(1){
		vsync();
		// ClearBall(&ball);
		// ClearPaddle(&p1);
		// ClearPaddle(&p2);

		// MoveBall(&ball);
		// DrawBall(&ball);

		// DrawPaddle(&p1);
		// DrawPaddle(&p2);
		
		// drawLine(10,4,230,4, setColor(31,31,31));
		// drawLine(10,156,230,156, setColor(31,31,31));

		drawLine(10,4,230,4, setColor(31,31,31));
		drawLine(10,156,230,156, setColor(31,31,31));
		
		drawLine(10,4,230,156, setColor(2,31,15));
		drawLine(10,156,230,4, setColor(2,15,31));

		drawRect(100,60,40, 40, setColor(31,5,12));

		// s32 x,y;
		// for(x = 0; x < SCREEN_W; ++x)
		// {
		// 	for( y = 0; y < SCREEN_H; ++y)
		// 	{
		// 		plotPixel(x,y,setColor((x&y)+t * 5, (x&y)+t*3, (x&y)+t));
				
		// 	}
		// }
		// ++t;
	}
	return 0;
}