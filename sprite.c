#include "sprite.h"

void renderSprite(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect* clip){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, clip, &dst );
}
