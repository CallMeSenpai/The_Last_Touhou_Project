#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "structs.h"

/*SDL for ubuntu 14.04 and probably lower
has been bugged and not patched.
mercurium has patched theirs and we have
used their edited version to include SDL,
which is why we're including a local library
rather than an the one installed via apt-get
*/
#include "include/SDL.h"


#include <time.h>
//timed movements are important

int main(){
  SDL_Window* window;
  //do we want video to record replays?
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  window = SDL_CreateWindow("Touhou- CStuy version",
			    0,
			    0,
			    800,
			    600,
			    0);
  if (! window){
    printf("Unable to create window.\n%s\n",SDL_GetError());
    exit(-1);
  }
  //program loop:SDL_PollEvent


  SDL_Delay(3000);
  SDL_DestroyWindow(window);
  SDL_Quit();  
  return 0; 
}
