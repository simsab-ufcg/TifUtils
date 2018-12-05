#include <bits/stdc++.h>
#include <tiffio.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
const int randomNumberLimit = 100000000;

// Returns random number with uniform probability in interval [0, i) 
int getUniformRandom(int i){
    return uniform_int_distribution<int>(0, i)(rng);
}

double getRandomDouble(){
    return  2.0 * getUniformRandom(randomNumberLimit) / randomNumberLimit;
}

/**

Random mode:

50%:
R*R*
*R*R
R*R*
*R*R

25%:
R***
***R
**R*
*R**

0%:
****
****
****
****

 * */

using Tiff = TIFF*;

Tiff openToWrite(string path, int width, int height){
    Tiff tif;
    tif = TIFFOpen(path.c_str(), "w8");

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH     , width); 
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH    , height);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE  , 64);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT   , 3);
	TIFFSetField(tif, TIFFTAG_COMPRESSION    , 1);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC    , 1);
	TIFFSetField(tif, TIFFTAG_ORIENTATION    , 1);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP   , 8);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT , 1);
	TIFFSetField(tif, TIFFTAG_XRESOLUTION    , 1);
	TIFFSetField(tif, TIFFTAG_YRESOLUTION    , 1);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG   , PLANARCONFIG_CONTIG   );

    return tif;
}

int main (int argc, char **argv){

    string outputName = string(argv[1]);
    int width;
    sscanf(argv[2], "%d", &width);
    int height = width;
    int blockSize;
    sscanf(argv[3], "%d", &blockSize);
    int denominator;
    sscanf(argv[4], "%d", &denominator);
    // complete random block will appear in the output with frequency of 1/denominator 

    Tiff output = openToWrite(outputName, width, height);

    double blockNum = getRandomDouble();
    double* line = new double[width];
    for(int row = 0; row < height; row++){
        int rowByBlock = row / blockSize;
        for(int column = 0; column < width; column++){
            int columnByBlock = column / blockSize;
            if (denominator == 0 || (rowByBlock + columnByBlock) % denominator != 0){
                line[column] = getRandomDouble();
            } else {
                line[column] = blockNum;
            }
        }
        TIFFWriteScanline(output, line, row);
    }

    TIFFClose(output);
	return 0;
}
