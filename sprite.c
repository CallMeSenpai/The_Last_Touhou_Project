#include "sprite.h"

void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect* clip){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, clip, &dst );
}

//assumes clip is not initialized
void init_reimu_test(sprite* sprite, SDL_Renderer* renderer) {
  sprite->frames = 4;
  sprite->current_frame = 0;
  sprite->clip = calloc(4, sizeof(SDL_Rect));
  sprite->texture = IMG_LoadTexture(renderer, "images/reimu_test_sheet.png");

  sprite->clip[0].x = 0;
  sprite->clip[0].y = 0;
  sprite->clip[0].w = 31;
  sprite->clip[0].h = 42;

  sprite->clip[1].x = 31;
  sprite->clip[1].y = 0;
  sprite->clip[1].w = 31;
  sprite->clip[1].h = 42;

  sprite->clip[2].x = 62;
  sprite->clip[2].y = 0;
  sprite->clip[2].w = 31;
  sprite->clip[2].h = 42;

  sprite->clip[3].x = 93;
  sprite->clip[3].y = 0;
  sprite->clip[3].w = 31;
  sprite->clip[3].h = 42;
}
