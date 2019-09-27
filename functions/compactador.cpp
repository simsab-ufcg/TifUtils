#include <bits/stdc++.h>
#include <tiffio.h>

using namespace std;

int main (int argc, char **argv){
	TIFF *input;
    int width, height;
	input = TIFFOpen(argv[1], "rm");

	TIFFGetField(input, TIFFTAG_IMAGEWIDTH  , &width); 
	TIFFGetField(input, TIFFTAG_IMAGELENGTH , &height);

    double values[width];

    double maxi = -1e9;
    double mini = 1e9;
	for (int row = 0; row < height; row++){
		TIFFReadScanline(input, values, row);
        for (int colunm = 0; colunm < width; colunm++) {
            if(!isnan(values[colunm])){
                maxi = max(maxi, values[colunm]);
                mini = min(mini, values[colunm]);
            }
		}
	}

    TIFF *output = TIFFOpen(argv[2], "w8m");

    TIFFSetField(output, TIFFTAG_IMAGEWIDTH     , width);
    TIFFSetField(output, TIFFTAG_IMAGELENGTH    , height);
	TIFFSetField(output, TIFFTAG_BITSPERSAMPLE  , 8);
	TIFFSetField(output, TIFFTAG_SAMPLEFORMAT   , 1);
	TIFFSetField(output, TIFFTAG_COMPRESSION    , 1);
	TIFFSetField(output, TIFFTAG_PHOTOMETRIC    , 7);
	TIFFSetField(output, TIFFTAG_ORIENTATION    , 1);
	TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(output, TIFFTAG_ROWSPERSTRIP   , 8);
	TIFFSetField(output, TIFFTAG_RESOLUTIONUNIT , 1);
	TIFFSetField(output, TIFFTAG_XRESOLUTION    , 1);
	TIFFSetField(output, TIFFTAG_YRESOLUTION    , 1);
	TIFFSetField(output, TIFFTAG_PLANARCONFIG   , PLANARCONFIG_CONTIG);

    unsigned char compress[width];

	for (int row = 0; row < height; row++){
		TIFFReadScanline(input, values, row);
        for (int colunm = 0; colunm < width; colunm++) {
            if(!isnan(values[colunm])){
                compress[colunm] = (((values[colunm] - mini) / (maxi - mini)) * 254) + 1;
            }else{
                compress[colunm] = 0;
            }
		}
        TIFFWriteScanline(output, compress, row);
	}  

	TIFFClose(input);
    TIFFClose(output);
	return 0;
}
