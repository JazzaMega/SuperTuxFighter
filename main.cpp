#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"

#define true 1
#define false 0
#define FLOOR_HEIGHT 430
#define TEMP_SPRITE_SIZE 64
#define TEMP_BG_SIZE 1200
#define GRAVITY 2
#define FRICTION .8
#define PLR_ACCEL 5

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

  int SCREEN_WIDTH = 640;
  int SCREEN_HEIGHT = 480;
  int LEVEL_WIDTH = 1200;
  int LEVEL_HEIGHT = 800;

  int colorkey, gameover;

  float PLR_wX,PLR_wY;
  float PLR_yvel, PLR_xvel;
  float LPX,LPY;
  float camX, camY;

  int PLR_jump = 0, PLR_grounded = 0;

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);

  /* set the title bar */
  SDL_WM_SetCaption("SDL Window", "SDL Window");

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
  temp  = IMG_Load("good_background_index.pcx");
  grass = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* set sprite position */
  rcPLRSprite.x = SCREEN_WIDTH / 2;
  rcPLRSprite.y = SCREEN_HEIGHT / 2;
  PLR_wX = 0;
  PLR_wY = 0;
  LPX=0;
  LPY=0;
  camX=0;
  camY=0;

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
      camY += 10;
    }
	if (keystate[SDLK_j] ) {
      camX += 10;
    }
	if (keystate[SDLK_k] ) {
      camY -= 10;
    }
	if (keystate[SDLK_l] ) {
      camX -= 10;
    }

	/* Apply velocity to the player */
	PLR_wY = PLR_wY + PLR_yvel;
	PLR_wX = PLR_wX + PLR_xvel;

  /* Apply friction to the player */

	if (PLR_xvel < -.5) {
		PLR_xvel = PLR_xvel + FRICTION;
	} else if (PLR_xvel > .5) {
		PLR_xvel = PLR_xvel - FRICTION;
	} else {
		PLR_xvel = 0;
	}

	/* Check for floor collision */
	if (PLR_wY > FLOOR_HEIGHT) {
		PLR_wY = FLOOR_HEIGHT;
		PLR_yvel = 0;
		PLR_jump = 0;
		PLR_grounded = 1;
	} else {
		PLR_grounded = 0;
	}

	/* Apply gravity to player velocity */
	PLR_yvel = PLR_yvel + GRAVITY;

	gcvt(camX, 6, buf);
	printf("\nbuffer is: %s", buf);

 /* Camera movement */
if (camX > 0) {
  camX = 0;
}
if (camY > 0) {
  camY = 0;
}
if (camX < (LEVEL_WIDTH - SCREEN_WIDTH) - ((LEVEL_WIDTH - SCREEN_WIDTH)*2)) {
  camX = (LEVEL_WIDTH - SCREEN_WIDTH) - ((LEVEL_WIDTH - SCREEN_WIDTH)*2);
}
if (camY < (LEVEL_HEIGHT - SCREEN_HEIGHT) - ((LEVEL_HEIGHT - SCREEN_HEIGHT)*2)) {
  camY = (LEVEL_HEIGHT - SCREEN_HEIGHT) - ((LEVEL_HEIGHT - SCREEN_HEIGHT)*2);
}

  /* Apply player and world movement */
rcGrass.x = camX;
rcGrass.y = camY;
rcPLRSprite.x = camX + PLR_wX;
rcPLRSprite.y = camY + PLR_wY;

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