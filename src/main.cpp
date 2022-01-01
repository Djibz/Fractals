#include <SDL2/SDL.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SortGUI",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                200,
                                200,
                                SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect a;
    a.x = 10;
    a.y = 10;
    a.w = 10;
    a.h = 10;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &a);

    SDL_RenderPresent(renderer);

    system("pause");
    return 0;
}