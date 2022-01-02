#include <SDL2/SDL.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

#define WIDTH 1920
#define HEIGHT 1080
unsigned int PRECISION = 255;

bool saveScreenshotBMP(string filepath, int nb, SDL_Window *window, SDL_Renderer *renderer) {
    string number = to_string(nb);
    for(int i=number.size(); i<6; ++i)
        filepath += '0';
    filepath += number;
    filepath += ".bmp";
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(window);
    if (infoSurface == NULL) {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    } else {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0) {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        } else {
            if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                delete[] pixels;
                return false;
            } else {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL) {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    delete[] pixels;
                    return false;
                }
                SDL_SaveBMP(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }
    return true;
}

int borned(double aX, double aY)
{
    double x = 0, y = 0;
    for(int i=0; i < PRECISION; ++i)
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
            int color = (2*255*pow) / PRECISION;
            if(color > 255)
                color = 255;
            if(pow > 0)
                SDL_SetRenderDrawColor(renderer, 0, color, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv)
{
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

    double goalX = -0.743643887037151;
    double goalY =  0.13182590420533;
    double zoom = (double)1/10;

    double ratio = (double)WIDTH/HEIGHT;

    double xMin = goalX - 1.5*ratio, xMax = goalX + 1.5*ratio, yMin = goalY - 1.5, yMax = goalY + 1.5;
    //double xMin = -2, xMax = 1, yMin = -1.5, yMax = 1.5;

    double coefX = double(WIDTH) / (xMax - xMin);
    double coefY = double(HEIGHT) / (yMax - yMin);

    for(int i=0; i<300; ++i)
    {
        if(true || i > 190)
            drawMandelbrot(xMin, xMax, yMin, yMax, coefX, coefY, renderer);
        saveScreenshotBMP("./images/big/", i, window, renderer);
        xMin += (goalX - xMin) * zoom;
        xMax += (goalX - xMax) * zoom;
        yMin += (goalY - yMin) * zoom;
        yMax += (goalY - yMax) * zoom;
        PRECISION *= 1 + zoom/6;
        coefX = double(WIDTH) / (xMax - xMin);
        coefY = double(HEIGHT) / (yMax - yMin);
    }

    system("pause");
    return 0;
}