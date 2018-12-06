#include "tiffio.h"
#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <set>

using namespace std;

// Usage: ./count2 file.tiff
int main(int argc, char* argv[]) {

    if(argc != 3) {
        cerr << "Invalid number of arguments!" << endl;
        return 0;
    }
    
    TIFF* tif;
    tif = TIFFOpen(argv[1], "rm");

    int width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    int CHUNK_SIZE = atoi(argv[2]);
    double* line = new double[width];

    if(CHUNK_SIZE == -1) CHUNK_SIZE = width*height;

    double value;
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

    printf("File %s\n", argv[1]);
    printf("Chunk size: %d\nChunks used: %lld\nValid elements: %lld\n", CHUNK_SIZE, noChunks, validElements);
    printf("Duplication: %.12f\n", 1.0 - ((double) answer) / ((double) validElements));

    TIFFClose(tif);
    return 0;
}