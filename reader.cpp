#include "tiffio.h"
#include <iostream>
#include <fstream>

using namespace std;

// Usage: ./reader pathToTiff.tif nameOfOutputFile.txt
int main(int argc, char* argv[]){
    
    if(argc != 3) {
        cerr << "Invalid number of arguments!" << endl;
        return 0;
    }
    
    TIFF* tif;
    tif = TIFFOpen(argv[1], "rm");

    int width, height;
    ofstream outputFile;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    double* tmp = new double[TIFFScanlineSize(tif)];
    outputFile.open(argv[2]);

    for(int i = 0; i < height; i++){
        TIFFReadScanline(tif, tmp, i);
        for(int j = 0; j < width; j++) {
            if(j != 0) outputFile << " ";
            outputFile << tmp[j]; 
        }
        outputFile << endl;
    }

    TIFFClose(tif);
    return 0;
}