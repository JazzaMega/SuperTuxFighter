#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TEMP_SPRITE_SIZE 64
#define TEMP_BG_SIZE 100
#define GRAVITY 2

int main ( int argc, char *argv[] )
{
  SDL_Surface *screen, *temp, *plrsprite, *grass;
  SDL_Rect rcPLRSprite, rcGrass;
  SDL_Event event;
  Uint8 *keystate;

  int colorkey, gameover;

  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);

  /* set the title bar */
  SDL_WM_SetCaption("SDL Move", "SDL Move");

  /* create window */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

  /* load sprite */
  temp   = SDL_LoadBMP("sprite.bmp");
  plrsprite = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* setup sprite colorkey and turn on RLE */
  colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  SDL_SetColorKey(plrsprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  /* load grass */
  temp  = SDL_LoadBMP("grass.bmp");
  grass = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* set sprite position */
  rcPLRSprite.x = 0;
  rcPLRSprite.y = 0;
  rcPLRSprite.xvel = 0;
  rcPLRSprite.yvel = 0;

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
          }
          break;
      }
    }

    /* handle sprite movement */
    keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_LEFT] ) {
      rcPLRSprite.x -= 2;
    }
    if (keystate[SDLK_RIGHT] ) {
      rcPLRSprite.x += 2;
    }
    if (keystate[SDLK_UP] ) {
      rcPLRSprite.y -= 2;
    }
    if (keystate[SDLK_DOWN] ) {
      rcPLRSprite.y += 2;
    }

    /* draw the background */
    for (int x = 0; x < SCREEN_WIDTH / TEMP_BG_SIZE; x++) {
      for (int y = 0; y < SCREEN_HEIGHT / TEMP_BG_SIZE; y++) {
        rcGrass.x = x * TEMP_BG_SIZE;
        rcGrass.y = y * TEMP_BG_SIZE;
        SDL_BlitSurface(grass, NULL, screen, &rcGrass);
      }
    }
    /* draw the sprite */
    SDL_BlitSurface(plrsprite, NULL, screen, &rcPLRSprite);

    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
	SDL_Flip(screen);
	SDL_Delay(16.667);
  }

  /* clean up */
  SDL_FreeSurface(plrsprite);
  SDL_FreeSurface(grass);
  SDL_Quit();

  return 0;
}