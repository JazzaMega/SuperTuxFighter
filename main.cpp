#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

#define true 1
#define false 0
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define FLOOR_HEIGHT 430
#define TEMP_SPRITE_SIZE 64
#define TEMP_BG_SIZE 1200
#define GRAVITY 2
#define FRICTION .45
#define PLR_ACCEL .8

/* Define keybinds */
#define btn_left SDLK_LEFT
#define btn_right SDLK_RIGHT
#define btn_jump SDLK_SPACE

/* Temporary debugging variables */
char buf[20];

int main ( int argc, char *argv[] )
{
  SDL_Surface *screen, *temp, *plrsprite, *grass;
  SDL_Rect rcPLRSprite, rcGrass;
  SDL_Event event;
  Uint8 *keystate;

  int colorkey, gameover;

  float PLR_yvel, PLR_xvel;

  int PLR_jump = 0, PLR_grounded = 0;

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);

  /* set the title bar */
  SDL_WM_SetCaption("SDL Move", "SDL Move");

  /* create window */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_RESIZABLE );

  /* load sprite */
  temp   = SDL_LoadBMP("sprite.bmp");
  plrsprite = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* setup sprite colorkey and turn on RLE */
  colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  SDL_SetColorKey(plrsprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  /* load grass */
  temp  = SDL_LoadBMP("good_background.bmp");
  grass = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* set sprite position */
  rcPLRSprite.x = 0;
  rcPLRSprite.y = 300;

  rcGrass.x = -500;
  rcGrass.y = 0;

  PLR_xvel = 0;
  PLR_yvel = 0;

  gameover = 0;

  /* message pump */
  while (!gameover)
  {

    /* look for an event */
    if (SDL_PollEvent(&event)) {
      /* an event was found */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          gameover = 1;
          break;

        /* handle the keyboard */
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
            case SDLK_q:
              gameover = 1;
              break;
			case btn_jump:
			  if (PLR_grounded || PLR_jump < 2) {
				PLR_jump = PLR_jump + 1;
				PLR_yvel = -25;
			  }
          }
          break;
      }
    }

    /* handle sprite movement */
    keystate = SDL_GetKeyState(NULL);
    if (keystate[btn_left] ) {
      if (PLR_xvel > -5) {
			    PLR_xvel -= PLR_ACCEL;
		}
    }
    if (keystate[btn_right] ) {
		if (PLR_xvel < 5) {
			    PLR_xvel += PLR_ACCEL;
		}
    }

	/* Debug background movement */
    if (keystate[SDLK_i] ) {
      rcGrass.y -= 10;
    }
	if (keystate[SDLK_j] ) {
      rcGrass.x -= 10;
    }
	if (keystate[SDLK_k] ) {
      rcGrass.y += 10;
    }
	if (keystate[SDLK_l] ) {
      rcGrass.x += 10;
    }

	/* Apply velocity to the player */
	rcPLRSprite.y = rcPLRSprite.y + PLR_yvel;
	rcPLRSprite.x = rcPLRSprite.x + PLR_xvel;

	if (PLR_xvel < .5) {
		PLR_xvel = PLR_xvel + FRICTION;
	} else if (PLR_xvel > -.5) {
		PLR_xvel = PLR_xvel - FRICTION;
	} else {
		PLR_xvel = 0;
	}

	/* Check for floor collision */
	if (rcPLRSprite.y > FLOOR_HEIGHT) {
		rcPLRSprite.y = FLOOR_HEIGHT;
		PLR_yvel = 0;
		PLR_jump = 0;
		PLR_grounded = 1;
	} else {
		PLR_grounded = 0;
	}

	/* Apply gravity to player velocity */
	PLR_yvel = PLR_yvel + GRAVITY;

	gcvt(PLR_jump, 6, buf);
	printf("\rbuffer is: %s", buf);
	//fflush(stdout); 

    /* draw the background */

    SDL_BlitSurface(grass, NULL, screen, &rcGrass);
    /* draw the sprite */
    SDL_BlitSurface(plrsprite, NULL, screen, &rcPLRSprite);

    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
	//SDL_Flip(screen);
	SDL_Delay(16.667);
  }

  /* clean up */
  SDL_FreeSurface(plrsprite);
  SDL_FreeSurface(grass);
  SDL_Quit();

  return 0;
}