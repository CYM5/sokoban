#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "SDL_image.h"

void print_map(int tab[12][12]){
    int i,j;
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
            printf("%d",tab[j][i]);
        }
        printf("\n");
    }
}

void print_map_c(char tab[12][12]){
    int i,j;
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
            printf("%c",tab[j][i]);
        }
        printf("\n");
    }
}
