#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include "SDL_image.h"
#include "constant.h"


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

    file = fopen("level/level2.txt", "r");
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    //Debut génération
    if(file != NULL){
        wall = IMG_Load("sprite/mur.jpg");
        mario = IMG_Load("sprite/mario_bas.gif");
        box = IMG_Load("sprite/caisse.jpg");
        goal = IMG_Load("sprite/objectif.png"); // Je charge mes sprites
        for(i=0;i<=12;i++){
            if(i!=0){
               fseek(file, 2, SEEK_CUR); //Je supprimes mes caractère de fin de chaine
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
    int i,j;
    int case_mario_x = 0, case_mario_y = 0;
    SDL_Rect begin_position, tmp_box, goal_position;
    SDL_Surface *empty = NULL;
    SDL_Event event;
    SDL_Surface *mario_up = NULL, *mario_down = NULL, *mario_left = NULL, *mario_right = NULL, *box = NULL;
    mario_down = IMG_Load("sprite/mario_bas.gif");
    mario_left = IMG_Load("sprite/mario_gauche.gif");
    mario_up = IMG_Load("sprite/mario_haut.gif");
    mario_right = IMG_Load("sprite/mario_droite.gif");
    box = IMG_Load("sprite/caisse.jpg");

    empty = SDL_CreateRGBSurface(SDL_HWSURFACE, 34, 34, 32, 0, 0, 0, 0); //Surface pour effacer l'ancien mario ou les anciennes boites
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
    Recup des infos importante de la map, position initial
    position des objectifs etc...
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
                    goal_position.x=j;
                    goal_position.y=i;
                    break;
            }
        }
    }
    case_mario_x = begin_position.x;
    case_mario_y = begin_position.y;
    begin_position.x=begin_position.x*BLOC_SIZE;
    begin_position.y=begin_position.y*BLOC_SIZE;


    /*
    DEBUT DU JEU
    */
    while(playing){
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                playing = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        playing = 0;
                        break;
                    case SDLK_UP:
                        if((map_int[case_mario_x][case_mario_y-1]== 2)&&(map_int[case_mario_x][case_mario_y-2]== 0)){
                            //DO NOTHING
                            // ICI on gère le cas ou la boite est collée au mur
                        }
                        else if(map_int[case_mario_x][case_mario_y-1] == 2){
                            //BOX
                            tmp_box.x=begin_position.x;
                            tmp_box.y=begin_position.y-(BLOC_SIZE*2);
                            SDL_BlitSurface(box, NULL, ecran, &tmp_box);
                            map_int[case_mario_x][case_mario_y-2]=2;
                            //MARIO
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);//Je vide l'ancienne case de mario
                            begin_position.y-=BLOC_SIZE; // Je met à jour la nouvelle position de mario
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);
                            map_int[case_mario_x][case_mario_y]=4; // Maj de la map pour déplacer mario à l'interieur
                            case_mario_y--; // Maj de la position de mario dans la map théorique
                            map_int[case_mario_x][case_mario_y]=1; //Maj de la postion de mario dans la map théorique
                            SDL_BlitSurface(mario_up, NULL, ecran, &begin_position); // Je déplace mario dans l'interface

                        }
                        else if(map_int[case_mario_x][case_mario_y-1]!= 0){ //Je test si on va dans un mur ou non
                            SDL_BlitSurface(empty, NULL, ecran, &begin_position);//Je vide l'ancienne case de mario
                            begin_position.y-=BLOC_SIZE; // Je met à jour la nouvelle position de mario
                            map_int[case_mario_x][case_mario_y]=4; // Maj de la map pour déplacer mario à l'interieur
                            case_mario_y--; // Maj de la position de mario dans la map théorique
                            map_int[case_mario_x][case_mario_y]=1; //Maj de la postion de mario dans la map théorique
                            SDL_BlitSurface(mario_up, NULL, ecran, &begin_position); // Je déplace mario dans l'interface
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
        playing=is_win(map_int);

        SDL_Flip(ecran);
    }
}

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
