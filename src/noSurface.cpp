#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

#define WIDTH 3840
#define HEIGHT 2160
/*#define WIDTH 1920
#define HEIGHT 1080*/
unsigned int PRECISION = 255*1;
unsigned int dark = 0;

struct BmpHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + (WIDTH * HEIGHT * 3);
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
    void save_on_file(ofstream& fout) {
        fout.write(this->bitmapSignatureBytes, 2);
        fout.write((char*)&this->sizeOfBitmapFile, sizeof(uint32_t));
        fout.write((char*)&this->reservedBytes, sizeof(uint32_t));
        fout.write((char*)&this->pixelDataOffset, sizeof(uint32_t));
    }
} bmpHeader;

struct BmpInfoHeader {
    uint32_t sizeOfThisHeader = 40;
    int32_t width = WIDTH; // in pixels
    int32_t height = HEIGHT; // in pixels
    uint16_t numberOfColorPlanes = 1; // must be 1
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0; // generally ignored
    int32_t horizontalResolution = 3780; // in pixel per meter
    int32_t verticalResolution = 3780; // in pixel per meter
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
    void save_on_file(ofstream& fout) {
        fout.write((char*)&this->sizeOfThisHeader, sizeof(uint32_t));
        fout.write((char*)&this->width, sizeof(int32_t));
        fout.write((char*)&this->height, sizeof(int32_t));
        fout.write((char*)&this->numberOfColorPlanes, sizeof(uint16_t));
        fout.write((char*)&this->colorDepth, sizeof(uint16_t));
        fout.write((char*)&this->compressionMethod, sizeof(uint32_t));
        fout.write((char*)&this->rawBitmapDataSize, sizeof(uint32_t));
        fout.write((char*)&this->horizontalResolution, sizeof(int32_t));
        fout.write((char*)&this->verticalResolution, sizeof(int32_t));
        fout.write((char*)&this->colorTableEntries, sizeof(uint32_t));
        fout.write((char*)&this->importantColors, sizeof(uint32_t));
    }
} bmpInfoHeader;

struct Pixel {
    uint8_t blue = 255;
    uint8_t green = 255;
    uint8_t red = 0;
    void save_on_file(ofstream& fout) {
        fout.write((char*)&this->blue, sizeof(uint8_t));
        fout.write((char*)&this->green, sizeof(uint8_t));
        fout.write((char*)&this->red, sizeof(uint8_t));
    }
} pixel;

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

int filter(int nb)
{
    return sin(nb * M_PI / (2*dark));
}

void drawMandelbrot(long double xMin, long double xMax, long double yMin, long double yMax, long double coefX, long double coefY, int nb)
{
    string filepath = "./images/238/vv";
    string number = to_string(nb);
    for(int i=number.size(); i<6; ++i)
        filepath += '0';
    filepath += number;
    filepath += ".bmp";
    ofstream fout(filepath, ios::binary);
    bmpHeader.save_on_file(fout);
    bmpInfoHeader.save_on_file(fout);
    Pixel pixel;
    for(int j=0; j<HEIGHT; ++j)
        for(int i=0; i<WIDTH; ++i)
        {
            long double x = (i / coefX) + xMin;
            long double y = (j / coefY) + yMin;
            int pow = borned(x, y);
            int color = (2*255*pow) / PRECISION;
            if(color >= 255)
                color = 255;
            if(color > dark)
            {
                pixel.blue = 0;
                pixel.green = color;
                pixel.red = 0;
            }
            else if (color == 0)
            {
                pixel.blue = 255;
                pixel.green = 255;
                pixel.red = 255;
            }
            else
            {
                color *= filter(color);
                pixel.blue = color;
                pixel.green = color;
                pixel.red = color;
            }
            pixel.save_on_file(fout);
        }
    cout << nb << endl;
    fout.close();
}

int main(int argc, char** argv)
{
    long double goalX = -0.743643887037151;
    long double goalY =  0.13182590420533;
    double zoom = (double)1/10;

    double ratio = (double)WIDTH/HEIGHT;

    long double xMin = goalX - 1.5*ratio, xMax = goalX + 1.5*ratio, yMin = goalY - 1.5, yMax = goalY + 1.5;
    //double xMin = -2, xMax = 1, yMin = -1.5, yMax = 1.5;

    long double coefX = double(WIDTH) / (xMax - xMin);
    long double coefY = double(HEIGHT) / (yMax - yMin);

    vector<thread> threads;
    
    auto start = chrono::system_clock::now();

    for(int i=0; i<300; ++i)
    {
        if(i == 238)
        {
            cout << "Precision  : " << PRECISION << endl;
            drawMandelbrot(xMin, xMax, yMin, yMax, coefX, coefY, i);
            //threads.push_back(thread(drawMandelbrot, xMin, xMax, yMin, yMax, coefX, coefY, i));
            /*for(int j=10; j<60; ++j)
            {
                dark = j;
                drawMandelbrot(xMin, xMax, yMin, yMax, coefX, coefY, j);
                cout << j << endl;
            }*/
        }
            
        xMin += (goalX - xMin) * zoom;
        xMax += (goalX - xMax) * zoom;
        yMin += (goalY - yMin) * zoom;
        yMax += (goalY - yMax) * zoom;
        PRECISION *= 1 + zoom/6;
        coefX = double(WIDTH) / (xMax - xMin);
        coefY = double(HEIGHT) / (yMax - yMin);

        if(threads.size() == 5)
        {
            for (auto &th : threads) {
                th.join();
            }
            threads.clear();
        }
    }

    for (auto &th : threads) {
        th.join();
    }

    

    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    cout << "Temps: " << elapsed_seconds.count() << "s\n";

    return 0;
}