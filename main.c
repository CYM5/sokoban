#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "SDL_image.h"
#include "constant.h"
#include "play.h"
#include "edit.h"
int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Rect init_menu;
    SDL_Event event;
    int playing=1;


    init_menu.x=0;
    init_menu.y=0;


    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(WINDOWS_H, WINDOW_L, 32, SDL_HWSURFACE |SDL_DOUBLEBUF);
    SDL_WM_SetCaption("SOKOBAN", NULL);

    menu= IMG_Load("sprite/menu.jpg");

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
                    case SDLK_ESCAPE: //On ferme le jeu
                        playing = 0;
                        break;
                    case SDLK_KP1:
                        play(ecran);
                        break;
                    case SDLK_KP2:
                        create_level(ecran);
                        break;
                }
            break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &init_menu);
        SDL_Flip(ecran);
    }
    SDL_FreeSurface(menu);
    SDL_Quit();

    return EXIT_SUCCESS;
}

