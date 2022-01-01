#include <SDL2/SDL.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;

#define WIDTH 1200
#define HEIGHT 800

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

void drawMandelbrot(double xMin, double xMax, double yMin, double yMax, double coefX, double coefY, SDL_Renderer * renderer)
{
    for(int i=0; i<WIDTH; ++i)
        for(int j=0; j<HEIGHT; ++j)
        {
            double x = (i / coefX) + xMin;
            double y = (j / coefY) + yMin;
            int pow = borned(x, y);
            int red = (pow*255) / 3;
            if(pow > 0)
                SDL_SetRenderDrawColor(renderer, 0, (3*pow)%256, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
    double xMin = -3.2, xMax = -0.2, yMin = -1, yMax = 1;

    double coefX = double(WIDTH) / (xMax - xMin);
    double coefY = double(HEIGHT) / (yMax - yMin);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SortGUI",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WIDTH,
                                HEIGHT,
                                SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    double xStep = 0.015;
    double yStep = 0.01;

    for(int i=0; i<100; ++i)
    {
        drawMandelbrot(xMin, xMax, yMin, yMax, coefX, coefY, renderer);
        xMin += xStep;
        xMax -= xStep;
        yMin += yStep;
        yMax -= yStep;
        coefX = double(WIDTH) / (xMax - xMin);
        coefY = double(HEIGHT) / (yMax - yMin);
    }

    system("pause");
    return 0;
}