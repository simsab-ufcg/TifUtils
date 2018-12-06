#include "tiffio.h"
#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <set>

#define CHUNK_SIZE 30000000
using namespace std;

// Usage: ./count2 file.tiff
int main(int argc, char* argv[]) {

    if(argc != 3) {
        cerr << "Invalid number of arguments!" << endl;
        return 0;
    }

    if(string(argv[2]) != "-v1" and string(argv[2]) != "-v2") {
        cerr << "This version does not exist." << endl;
        return 0;
    }
    
    TIFF* tif;
    tif = TIFFOpen(argv[1], "rm");

    int width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    double* line = new double[width];

    if( string(argv[2]) == "-v1" ) {
        set<double> distinct;
        long long valid = 0;
        for(int row = 0; row < height; row++){
            TIFFReadScanline(tif, line, row);
            for(int column = 0; column < width; column++){
                if(!isnan(line[column])){
                    distinct.insert(line[column]);
                    valid += 1;
                }
            }
        }

        printf("File %s\nDuplication: %.12lf\n", argv[1], 1 - 1.0 * distinct.size() / valid);
    }
    else {
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

        printf("File %s\nChunks used: %lld\n", argv[1], noChunks);
        printf("Valid elements: %lld\n", validElements);
        printf("Duplication: %.12f\n", 1.0 - ((double) answer) / ((double) validElements));
    }

    TIFFClose(tif);
    return 0;
}