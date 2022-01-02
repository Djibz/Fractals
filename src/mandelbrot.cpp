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

void HSVcolor(float H, SDL_Renderer *renderer){
    float S = 100, V = 100;
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        cout<<"The givem HSV values are not in valid range"<<endl;
        return;
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    int R = (r+m)*255;
    int G = (g+m)*255;
    int B = (b+m)*255;
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
}

int borned(long double aX, long double aY)
{
    long double x = 0, y = 0;
    for(unsigned int i=0; i < PRECISION*2; ++i)
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

void drawMandelbrot(long double xMin, long double xMax, long double yMin, long double yMax, long double coefX, long double coefY, SDL_Renderer * renderer)
{
    for(int i=0; i<WIDTH; ++i)
        for(int j=0; j<HEIGHT; ++j)
        {
            long double x = (i / coefX) + xMin;
            long double y = (j / coefY) + yMin;
            int pow = borned(x, y);
            int color = (2*255*pow) / PRECISION;
            if(color >= 255)
                color = 255;
            if(pow > 0)
                //HSVcolor(color, renderer);
                SDL_SetRenderDrawColor(renderer, 0, color, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

    long double goalX = -0.743643887037151;
    long double goalY =  0.13182590420533;
    double zoom = (double)1/10;

    double ratio = (double)WIDTH/HEIGHT;

    long double xMin = goalX - 1.5*ratio, xMax = goalX + 1.5*ratio, yMin = goalY - 1.5, yMax = goalY + 1.5;
    //double xMin = -2, xMax = 1, yMin = -1.5, yMax = 1.5;

    long double coefX = double(WIDTH) / (xMax - xMin);
    long double coefY = double(HEIGHT) / (yMax - yMin);

    for(int i=0; i<400; ++i)
    {
        cout << '\r' << i << endl;
        if(true || i > 190)
            drawMandelbrot(xMin, xMax, yMin, yMax, coefX, coefY, renderer);
        saveScreenshotBMP("./images/black/", i, window, renderer);
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