#include <SDL2/SDL.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;

int borned(double aX, double aY)
{
    double x = 0, y = 0;
    for(int i=0; i<50; ++i)
    {
        double nX, nY;
        nX = (x * x) - (y * y) + aX;
        nY = (2 * x * y) + aY;
        x = nX;
        y = nY;
        if(((x * x) + (y * y)) > 4)
            return i+1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    int width = 1200;
    int height = 800;

    double coef = double(width) / 3;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SortGUI",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width,
                                height,
                                SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for(int i=0; i<width; ++i)
        for(int j=0; j<height; ++j)
        {
            double x = (i / coef) - 2;
            double y = (j / coef) - 1;
            int pow = borned(x, y);
            int red = (pow*255) / 3;
            if(pow > 0)
                SDL_SetRenderDrawColor(renderer, (3*pow)%256, (1*pow)%256, (10*pow)%256, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderPresent(renderer);

    system("pause");
    return 0;
}