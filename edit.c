#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "SDL_image.h"
#include "constant.h"
#include "tools.h"

void create_level(SDL_Surface* ecran){
    int continuer = 1;
    SDL_Event event;
    int map[12][12]={0};
    int click_loc[2]={0};

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_Flip(ecran);

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONUP: //click
                if (event.button.button == SDL_BUTTON_LEFT){
                    click_loc[0]=event.button.x;
                    click_loc[1]=event.button.y; //taking the pixel clicked by the user
                    return_map_position(click_loc); //return map position to a simple format

                    if(map[click_loc[0]][click_loc[1]]==4){ //The editor work with multiple click, so I incr the value
                        map[click_loc[0]][click_loc[1]]=0;
                    }
                    else{
                        map[click_loc[0]][click_loc[1]]++;
                    }
                    display_map(ecran,map);

                }
                else{
                    continuer=0;
                }

                break;
        }


    }
    save_map(map);
}


void display_map(SDL_Surface* ecran, int map[12][12]){
    SDL_Surface *wall = NULL, *mario = NULL, *box = NULL, *goal = NULL;
    SDL_Rect position;
    position.x=0;
    position.y=0;
    int i,j;
    wall = IMG_Load("sprite/mur.jpg");
    mario = IMG_Load("sprite/mario_bas.gif");
    box = IMG_Load("sprite/caisse.jpg");
    goal = IMG_Load("sprite/objectif.png");
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    for(i=0;i<=12;i++){
        for(j=0;j<12;j++){
            switch(map[j][i]){
                case 0:
                    SDL_BlitSurface(wall, NULL, ecran, &position);
                    break;
                case 1:
                    SDL_BlitSurface(mario, NULL, ecran, &position);
                    break;
                case 2:
                    SDL_BlitSurface(box, NULL, ecran, &position);
                    break;
                case 3:
                    SDL_BlitSurface(goal, NULL, ecran, &position);
                    break;
                case 4:
                    break;
                default:
                    break;
                }
                position.x+=BLOC_SIZE;
            }
            SDL_Flip(ecran);
            position.y+=BLOC_SIZE;
            position.x=0;
        }

    SDL_FreeSurface(mario);
    SDL_FreeSurface(wall);
    SDL_FreeSurface(box);
    SDL_FreeSurface(goal);
}

void return_map_position(int tab[2]){
    /*
    The logical map is 12*12 square, in this function a take the position
    of the pixel clicked by the user and I put it in a sub square
    */

    if(tab[0]<34){
        tab[0]=0;
    }
    else if(tab[0]<68){
        tab[0]=1;
    }
    else if(tab[0]<102){
        tab[0]=2;
    }
    else if(tab[0]<136){
        tab[0]=3;
    }
    else if(tab[0]<170){
        tab[0]=4;
    }
    else if(tab[0]<204){
        tab[0]=5;
    }
    else if(tab[0]<238){
        tab[0]=6;
    }
    else if(tab[0]<272){
        tab[0]=7;
    }
    else if(tab[0]<306){
        tab[0]=8;
    }
    else if(tab[0]<340){
        tab[0]=9;
    }
    else if(tab[0]<374){
        tab[0]=10;
    }
    else{
        tab[0]=11;
    }

    if(tab[1]<34){
        tab[1]=0;
    }
    else if(tab[1]<68){
        tab[1]=1;
    }
    else if(tab[1]<102){
        tab[1]=2;
    }
    else if(tab[1]<136){
        tab[1]=3;
    }
    else if(tab[1]<170){
        tab[1]=4;
    }
    else if(tab[1]<204){
        tab[1]=5;
    }
    else if(tab[1]<238){
        tab[1]=6;
    }
    else if(tab[1]<272){
        tab[1]=7;
    }
    else if(tab[1]<306){
        tab[1]=8;
    }
    else if(tab[1]<340){
        tab[1]=9;
    }
    else if(tab[1]<374){
        tab[1]=10;
    }
    else{
        tab[1]=11;
    }
}

void save_map(int map[12][12]){
    /*
    Function to save the new map
    */
     FILE* new_file = NULL;
     int i,j;

     new_file=fopen("level/new_level.txt", "w");

    if(new_file != NULL){

        for(i=0;i<12;i++){
            for(j=0;j<12;j++){
                fprintf(new_file, "%d", map[j][i]);
            }
            fputc('\n',new_file);
        }
        fclose(new_file);
    }
    else{
        printf("ERROR OPENING FILE");
    }


}
