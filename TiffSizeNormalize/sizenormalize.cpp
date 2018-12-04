#include <bits/stdc++.h>
#include <tiffio.h>
#define pixelSize 30
#define NaN (-sqrt(-1.0))

using namespace std;

typedef struct Point{
    double x, y;
}Point;

int main (int argc, char **argv){

    // constants
    const int INPUT_BASE_INDEX[2] = {1, 4};
    const int INPUT_COORD_VALUES[2][2] = {{2, 3}, {5, 6}};

    const int OUTPUT_BASE_INDEX = 7;

    // valid arguments
    if(argc != 8){
        cerr << "Arguments insufficients\n";
        exit(0);
    }

    TIFF *input, *normalize, *output;
    
    input = TIFFOpen( argv[INPUT_BASE_INDEX[0]], "rm");
    normalize = TIFFOpen( argv[INPUT_BASE_INDEX[1]], "rm");
    output = TIFFOpen( argv[OUTPUT_BASE_INDEX], "w8m");

    Point points[2];

    for(int i = 0; i < 2; i++){
        
        double x = atof(argv[INPUT_COORD_VALUES[i][0]]);
        double y = atof(argv[INPUT_COORD_VALUES[i][1]]);
        
        points[i] = {x, y};
    }

    int imageLength, imageWidth;

    TIFFGetField(normalize, TIFFTAG_IMAGEWIDTH  , &imageWidth); 
    TIFFGetField(normalize, TIFFTAG_IMAGELENGTH , &imageLength);

    TIFFSetField(output, TIFFTAG_IMAGEWIDTH     , imageWidth); 
    TIFFSetField(output, TIFFTAG_IMAGELENGTH    , imageLength);
    TIFFSetField(output, TIFFTAG_BITSPERSAMPLE  , 64);
    TIFFSetField(output, TIFFTAG_SAMPLEFORMAT   , 3);
    TIFFSetField(output, TIFFTAG_COMPRESSION    , 1);
    TIFFSetField(output, TIFFTAG_PHOTOMETRIC    , 1);
    TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(output, TIFFTAG_ROWSPERSTRIP   , 1);
    TIFFSetField(output, TIFFTAG_RESOLUTIONUNIT , 1);
    TIFFSetField(output, TIFFTAG_XRESOLUTION    , 1);
    TIFFSetField(output, TIFFTAG_YRESOLUTION    , 1);
    TIFFSetField(output, TIFFTAG_PLANARCONFIG   , PLANARCONFIG_CONTIG);

    double write_line[imageWidth];
    
    for(register int i = 0; i < imageWidth; i++){
        write_line[i] = NaN;
    }
    
    for(register int i = 0; i < imageLength; i++){
        if( TIFFWriteScanline(output, write_line, i) < 0){
            TIFFClose(input);
            TIFFClose(normalize);
            TIFFClose(output);
            exit(4 << 3);
        }
    }

    int offsetX = (points[0].x - points[1].x) / pixelSize;   
    int offsetY = (points[1].y - points[0].y) / pixelSize;

    int inputLength, inputWidth;

    TIFFGetField(input, TIFFTAG_IMAGEWIDTH  , &inputWidth); 
    TIFFGetField(input, TIFFTAG_IMAGELENGTH , &inputLength);

    double read_line[inputWidth];

    cout << offsetX << " " << offsetY << endl;

    for(register int i = max(0, offsetY); i < imageLength && (i - offsetY) < inputLength; i++){

        if( TIFFReadScanline(input, read_line, i - offsetY) < 0 ){
            TIFFClose(input);
            TIFFClose(normalize);
            TIFFClose(output);
            exit(2 << 3);
        }

        for(register int j = 0; j < imageWidth; j++){
            write_line[j] = NaN;
            if(j >= offsetX && (j - offsetX) < inputWidth){
                write_line[j] = read_line[j - offsetX];
            }
        }

        if( TIFFWriteScanline(output, write_line, i) < 0 ){
            TIFFClose(input);
            TIFFClose(normalize);
            TIFFClose(output);
            exit(4 << 3);
        }
    }

    TIFFClose(input);
    TIFFClose(normalize);
    TIFFClose(output);

	return 0;
}
