#include "tiffio.h"
#include <set>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

// Usage: ./count name
int main(int argc, char* argv[]){
    
    if(argc != 2) {
        cerr << "Invalid number of arguments!" << endl;
        return 0;
    }
    
    TIFF* tif;
    tif = TIFFOpen(argv[1], "rm");

    int width, height;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

    double* line = new double[width];
    set<double> distinct;
    int valid = 0;
    for(int row = 0; row < height; row++){
        TIFFReadScanline(tif, line, row);
        for(int column = 0; column < width; column++){
            if(!isnan(line[column])){
                distinct.insert(line[column]);
                valid += 1;
            }
        }
    }

    printf("File %s Duplication: %.12lf\n", argv[1], 1 - 1.0 * distinct.size() / valid);
    TIFFClose(tif);
    return 0;
}