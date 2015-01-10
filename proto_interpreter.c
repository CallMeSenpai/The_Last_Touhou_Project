#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void read_dat(char* file)
int main() {
  FILE *f = fopen("1e.dat","r");
  char* line = NULL; //with \n
  int len = 0; //size_t
  int read; //ssize_t
  
  char name[3];
  int id;
  int hp;
  int x;
  int y;

  while ((read = getline(&line,&len,f)) != -1) {
    printf("Retrieved length %d\n", read);
    printf("%s", line); //this has \n
    
    if (!strcmp("mob\n", line)) {
      strcpy(name,line);
      printf("name: %s", name); //still has the \n

    }
    else {
      //char* left;
      //char* right;
     
      char* token;
      //token = strsep(&line,"=");
      token = strsep(&line,"=");
      printf("token %s\n", token); //isolated value
      token = strsep(&line,"=");
      printf("int %d\n",atoi(token)); //isolated number

      

    }
  }

  fclose(f);
  //  if (line) {
  //free(line);
  //}
  
  
  
  
  return 0;
}
