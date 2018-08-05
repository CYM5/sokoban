#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include "SDL_image.h"
#include "constant.h"
#include "tools.h"


void load_level(SDL_Surface* ecran, char map[12][12]){
    /*
    This function receive the ptr screen, and will load a level file
    to print the level in my screen
    */
    FILE* file = NULL;
    SDL_Surface *wall = NULL, *mario = NULL, *box = NULL, *goal = NULL;
    SDL_Rect position;
    char carac = '0';
    position.x=0;
    position.y=0;


    int i,j=0;
    char level_line[12]="";

    file = fopen("level/new_level.txt", "r");
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    //Debut génération
    if(file != NULL){
        wall = IMG_Load("sprite/mur.jpg");
        mario = IMG_Load("sprite/mario_bas.gif");
        box = IMG_Load("sprite/caisse.jpg");
        goal = IMG_Load("sprite/objectif.png"); // Load sprites
        for(i=0;i<=12;i++){
            if(i!=0){
               fseek(file, 2, SEEK_CUR); //Delete end caracter of each line
            }
            fgets(level_line,MAX_level_line,file);
            for(j=0;j<12;j++){
                carac= level_line[j];
                map[i][j]=carac;
                switch(carac){
                    case '0':
                        SDL_BlitSurface(wall, NULL, ecran, &position);
                        break;
                    case '1':
                        SDL_BlitSurface(mario, NULL, ecran, &position);
                        break;
                    case '2':
                        SDL_BlitSurface(box, NULL, ecran, &position);
                        break;
                    case '3':
                        SDL_BlitSurface(goal, NULL, ecran, &position);
                        break;
                    case '4':
                        break;
                    default:
                        break;
                }
                position.x+=BLOC_SIZE;
            }
            SDL_Flip(ecran);
            position.y+=BLOC_SIZE;
            position.x=0;
        } //Fin de la boucle


        fclose(file);
        SDL_FreeSurface(mario);
        SDL_FreeSurface(wall);
        SDL_FreeSurface(box);
        SDL_FreeSurface(goal);

    }
}

void play(SDL_Surface* ecran){
    unsigned int playing = 1;
    int nb_goal = 0;
    int i,j,cmpt=0,cmpt_goal;
    int case_mario_x = 0, case_mario_y = 0;
    SDL_Rect begin_position, tmp_box, *goal_position = NULL, goal_position_to_screen;
    SDL_Surface *empty = NULL;
    SDL_Event event;
    SDL_Surface *mario_up = NULL, *mario_down = NULL, *mario_left = NULL, *mario_right = NULL, *box = NULL, *box_ok = NULL;
    mario_down = IMG_Load("sprite/mario_bas.gif");
    mario_left = IMG_Load("sprite/mario_gauche.gif");
    mario_up = IMG_Load("sprite/mario_haut.gif");
    mario_right = IMG_Load("sprite/mario_droite.gif");
    box = IMG_Load("sprite/caisse.jpg");
    box_ok = IMG_Load("sprite/caisse_ok.jpg");

    empty = SDL_CreateRGBSurface(SDL_HWSURFACE, 34, 34, 32, 0, 0, 0, 0); //Surface to delete the old mario
    SDL_FillRect(empty, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    int map_int[12][12]={0};
    char map[12][12]={'0'};
    load_level(ecran, map);
    /*
    CONVERTION DE LA MAP EN INT
    */
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
            switch (map[i][j])
            {
                case '0':
                    map_int[j][i] = 0;
                    break;
                case '1':
                    map_int[j][i] = 1;
                    break;
                case '2':
                    map_int[j][i] = 2;
                    break;
                case '3':
                    map_int[j][i] = 3;
                    break;
                case '4':
                    map_int[j][i] = 4;
                    break;
            }

        }

    }
    /*
    FIN DE LA CONVERTION
    */

    /*
    To have the begin position of mario
    and the number of obj
    */
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
            switch(map_int[i][j]){
                case 1:
                    begin_position.x=i;
                    begin_position.y=j;
                    break;
                case 3:
                    nb_goal++;
                    break;
            }
        }
    }
    case_mario_x = begin_position.x;
    case_mario_y = begin_position.y;
    begin_position.x=begin_position.x*BLOC_SIZE;
    begin_position.y=begin_position.y*BLOC_SIZE;

    /*
    TO HAVE ALL POSITION OF OBJ
    */
    goal_position = malloc(nb_goal*sizeof(SDL_Rect));

    if(goal_position==NULL){
        exit(1);
    }
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
                if(map_int[i][j]==3){
                    goal_position[cmpt].x=i;
                    goal_position[cmpt].y=j;
                    cmpt++;
                }
        }
    }
    /*
    END
    */
    /*
    BEGIN
    */
    while(playing){

        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                playing = 0;
                break;
            case SDL_KEYDOWN:
                cmpt_goal=0;
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        playing = 0;
                        break;
                    case SDLK_UP:
                        if((map_int[case_mario_x][case_mario_y-1]== 2)&&(map_int[case_mario_x][case_mario_y-2]== 0)){
                            //DO NOTHING
                            //WHEN A BOX IS NEAR TO A WALL
                        }
                        else if(map_int[case_mario_x][case_mario_y-1] == 2){
                            //MOVE THE BOX FIRST
                            tmp_box.x=begin_position.x;
                            tmp_box.y=begin_position.y-(BLOC_SIZE*2);
                            SDL_BlitSurface(box, NULL, ecran, &tmp_box);
                            map_int[case_mario_x][case_mario_y-2]=2;
                            //MARIO
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);//Empty old mario position
                            begin_position.y-=BLOC_SIZE; // Update the new position
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            map_int[case_mario_x][case_mario_y]=4; //Update the old mario position to empty the case
                            case_mario_y--; // Update mario pos in the teorical map
                            map_int[case_mario_x][case_mario_y]=1; //Update the new mario position
                            SDL_BlitSurface(mario_up, NULL, ecran, &begin_position); // Move mario

                        }
                        else if(map_int[case_mario_x][case_mario_y-1]!= 0){
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.y-=BLOC_SIZE;
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_y--;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_up, NULL, ecran, &begin_position);
                        }
                        break;
                    case SDLK_DOWN:
                        if((map_int[case_mario_x][case_mario_y+1]== 2)&&(map_int[case_mario_x][case_mario_y+2]== 0)){
                            //DO NOTHING
                        }
                        else if(map_int[case_mario_x][case_mario_y+1]== 2){
                            //BOX
                            tmp_box.x=begin_position.x;
                            tmp_box.y=begin_position.y+(BLOC_SIZE*2);
                            SDL_BlitSurface(box, NULL, ecran, &tmp_box);
                            map_int[case_mario_x][case_mario_y+2]=2;
                            //MARIO
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.y+=BLOC_SIZE;
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_y++;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_down, NULL, ecran, &begin_position);
                            print_map(map_int);
                        }
                        else if(map_int[case_mario_x][case_mario_y+1]!= 0){
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.y+=BLOC_SIZE;
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_y++;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_down, NULL, ecran, &begin_position);
                        }
                        break;
                    case SDLK_RIGHT:
                        printf("NEW MAP \n");
                        print_map(map_int);
                        if((map_int[case_mario_x+1][case_mario_y]== 2) && (map_int[case_mario_x+2][case_mario_y]== 0)){
                            //DO NOTHING
                        }
                        else if(map_int[case_mario_x+1][case_mario_y]== 2){
                            //BOX
                            tmp_box.x=begin_position.x+(BLOC_SIZE*2);
                            tmp_box.y=begin_position.y;
                            SDL_BlitSurface(box, NULL, ecran, &tmp_box);
                            map_int[case_mario_x+2][case_mario_y]=2;
                            //MARIO
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.x+=BLOC_SIZE;
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_x++;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_right, NULL, ecran, &begin_position);
                        }
                        else if(map_int[case_mario_x+1][case_mario_y]!= 0){
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.x+=BLOC_SIZE;
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_x++;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_right, NULL, ecran, &begin_position);
                        }
                        break;
                    case SDLK_LEFT:
                        if((map_int[case_mario_x-1][case_mario_y]== 2) && (map_int[case_mario_x-2][case_mario_y]== 0)){
                            //DO NOTHING
                        }
                        else if(map_int[case_mario_x-1][case_mario_y]== 2){
                            //BOX
                            tmp_box.x=begin_position.x-(BLOC_SIZE*2);
                            tmp_box.y=begin_position.y;
                            SDL_BlitSurface(box, NULL, ecran, &tmp_box);
                            map_int[case_mario_x-2][case_mario_y]=2;
                            //MARIO
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.x-=BLOC_SIZE;
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_x--;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_left, NULL, ecran, &begin_position);
                        }
                        else if(map_int[case_mario_x-1][case_mario_y]!= 0){
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            begin_position.x-=BLOC_SIZE;
                            map_int[case_mario_x][case_mario_y]=4;
                            case_mario_x--;
                            map_int[case_mario_x][case_mario_y]=1;
                            SDL_BlitSurface(mario_left, NULL, ecran, &begin_position);
                        }
                        break;
                }
            break;
        }
        //Test to see is the game is win or not, we check if there is a box on any goal position
        for(i=0;i<nb_goal;i++){
            //printf("GOAL OBJ -> %d \n",map_int[goal_position[i].x][goal_position[i].y]);
            if(map_int[goal_position[i].x][goal_position[i].y]==2){
                goal_position_to_screen.x = goal_position[i].x * BLOC_SIZE;
                goal_position_to_screen.y = goal_position[i].y * BLOC_SIZE;
                SDL_BlitSurface(box_ok, NULL, ecran, &goal_position_to_screen);
                cmpt_goal++;
            }
        }
        if(cmpt_goal==nb_goal){
            playing=0;
        }

        SDL_Flip(ecran);
    }
    free(goal_position);
    SDL_FreeSurface(mario_down);
    SDL_FreeSurface(mario_right);
    SDL_FreeSurface(mario_up);
    SDL_FreeSurface(mario_left);
    SDL_FreeSurface(box);
    SDL_FreeSurface(box_ok);
}


int is_win(int map[12][12]){
    /*
    Function wich search an empty obj in the map,
    if there is no empty obj so the player win
    */
    int i,j;
    for(i=0;i<12;i++){
        for(j=0;j<12;j++){
            if(map[i][j]==3){
                return 1;

            }
        }
    }
    return 0;
}
