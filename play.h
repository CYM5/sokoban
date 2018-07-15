#ifndef PLAY_H_INCLUDED
#define PLAY_H_INCLUDED

void load_level(SDL_Surface* ecran, char map[12][12]);
void play(SDL_Surface* ecran);
int is_win(int map[12][12]);
#endif // PLAY_H_INCLUDED
