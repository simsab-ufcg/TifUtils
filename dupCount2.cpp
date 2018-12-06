#include "tiffio.h"
#include <iostream>
#include <math.h>
#include <map>

#define CHUNK_SIZE 30000000
using namespace std;

// Usage: ./count2 file.tiff
int main(int argc, char* argv[]) {

    if(argc != 2) {
        cerr << "Invalid number of arguments!" << endl;
        return 0;
    }
    
    TIFF* tif;
    tif = TIFFOpen(argv[1], "rm");

    int width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    double value;
    double* line = new double[width];
    map<double, bool> elementsInChunk;
    long long sizeOfInterval = 0LL, answer = 0LL, validElements = 0LL, noChunks = 0LL;

    for(int row = 0; row < height; row++) {
        TIFFReadScanline(tif, line, row);
        for(int column = 0; column < width; column++) {

            value = line[column];
            if( !isnan(value) ) {
                validElements++;
                elementsInChunk[ value ] = true;
            }
        }

        if( elementsInChunk.size() >= CHUNK_SIZE ) {
            answer += ((long long) elementsInChunk.size());
            elementsInChunk.clear();
            noChunks++;
        }
    }

    if(elementsInChunk.size() > 0) {
        answer += ((long long) elementsInChunk.size());
        elementsInChunk.clear();
        noChunks++;
    }

    printf("Number of chunks used: %lld\n", noChunks);
    printf("Number of valid elements: %lld\n", validElements);
    printf("Duplication: %.12f\n", 1.0 - ((double) answer) / ((double) validElements));

    TIFFClose(tif);
    return 0;
}